#version 460
#shadertype VERTEX_SHADER
#shadertype FRAGMENT_SHADER

#include "common.inl.glsl"
#include "shader_globals.inl.glsl"


layout(binding = 0) uniform WorldData {
    MultiverseWorldConstants world;
};
layout(binding = 1) uniform CameraData {
    CameraConstants camera;
};
layout(binding = 2) uniform SpaceObjects {
    SpaceObjectGPU space_objects[MAX_UBO_SPC_OBJ_STRUCT_COUNT];
};

layout(binding = 3) uniform sampler2D blueNoiseTex;

layout(location = 0) c_attrib vec3 vPosition;
layout(location = 1) c_attrib vec3 rel_position;
layout(location = 2) c_attrib vec4 inv_rotquat;




#ifdef HX_VERTEX_SHADER

layout (location = 0) in vec3 pos;

void main(){
    SpaceObjectGPU obj = space_objects[gl_InstanceIndex];
    rel_position = obj.position.xyz;
    inv_rotquat = obj.rotationInv;

    vec3 vertpos = triangle_strip_cube_vertices[
		triangle_strip_cube_indices[ gl_VertexIndex ]
	];
    // vertpos *= 1.25;

    vertpos = mulvq(obj.rotation, vertpos);

    vPosition = vertpos;
    vec3 finalpos = vertpos - obj.position.xyz;
    gl_Position = camera.projection * camera.view * vec4(finalpos, 1.0);
}

#endif






#ifdef HX_FRAGMENT_SHADER

layout(location = 0) out vec4 FragColor;

float sdTorus(vec3 p, vec2 t, float th){
    vec2 q = vec2(length(p.xz)-t.x,p.y);
    return max(
        length(q) - t.y,
        -length(q+vec2(t.y*(1.0-th), 0)) + t.y*th
    );
}

vec2 getTorusUV(vec3 p, vec2 t){
    return vec2(
        atan(p.z, p.x),
        atan(p.y, length(p.xz) - t.x)
    ) / PI * 0.5 + 0.5;
}

float makeTorusCol(vec2 uv){
    vec2 guv = uv*2.0-1.0;
    float col1 = easeInExpoFac(max(pow(guv.y, 7.0), 0.0), 1.5);
    float col2 = easeInExpoFac(max(pow(-guv.y, 5.0), 0.0), 1.7);
    float col = col1 + col2;
    col *= 1.0-easeInCirc(min(1.3-abs(guv.y+0.2), 1.0));
    return col;
}

float getTorusDensity(vec3 p, vec2 torus) {
    float radius = torus.x+torus.y;
    float shape = sdTorus(p, torus, 0.8);
    float density = makeTorusCol(getTorusUV(p, torus));
    shape = max(smoothstep(radius/30.0, 0.0, shape), 0.0);
    return max(density * shape, 0.0);
}

vec3 cosmicWebColor(float value) {
    vec3 darkPurple = vec3(0.2, 0.0, 0.4);
    vec3 purple = vec3(0.5, 0.0, 0.5);
    vec3 magenta = vec3(0.8, 0.0, 0.4);
    vec3 orange = vec3(1.0, 0.5, 0.1);
    vec3 yellow = vec3(1.0, 1.0, 0.1);

    vec3 c = vec3(0);
    if (value < 0.25) c = mix(darkPurple, purple, value / 0.25);
    else if (value < 0.5) c = mix(purple, magenta, (value - 0.25) / 0.25);
    else if (value < 0.75) c = mix(magenta, orange, (value - 0.5) / 0.25);
    else c = mix(orange, yellow, (value - 0.75) / 0.25);

    return c;// * sqrt(value);
}

vec4 getMatterDensity(vec3 eye, vec3 dir, float radius, vec3 reye, float jit){
    float steps = 64.0;
    float startdist = 0.0;
    float transition_range = 12.0;

    vec4 acc = vec4(0);
    vec3 cp = eye;
    float stepsize = (radius*2.0)/steps;// + (jit*0.025);
    float globaldistfac = smoothstep(radius*2.5, radius*transition_range*1.1, length(reye));

    for (float i=0.0; i<steps; ++i){
        if (acc.a >= 1.0)break;
        float dist = length(reye-cp);
        float density = getTorusDensity(cp, vec2(0.5)) * 10.0;// / steps * 100.0;
        density = mix(density, density*(pow(hash13(cp*2.0e4), 100.0) * 7.0 + 1.0), 1.0-saturate(dist/radius * 2.0/transition_range)); //glitter
        // density = mix(density, pow(density, 10.0), smoothstep(radius*0.1, radius*0.01, dist));
        //density = density * (1.0-saturate(0.01/dist));//mix(density, pow(density, 10.0), saturate(0.1/dist));
        density = mix(density, pow(density, 4.0), globaldistfac);

        vec4 col = vec4(cosmicWebColor(density), density*0.5);
        acc.xyz += col.xyz * col.a;
        acc.a += col.a;

        // acc += density * (1.0-acc);//(1.0 - i/steps);
        cp += dir * stepsize;
    }

    return max(acc, 0.);
}




vec3 rayDirection2(float fieldOfView, vec2 size, vec2 fragCoord) {
    vec2 xy = fragCoord*2. - size;
    float z = min(size.y,size.x) / tan(radians(fieldOfView) / 2.0);
    return normalize(vec3(xy, -z));
}



void main(){
    vec2 C = gl_FragCoord.xy;
    C.y = camera.resolution.y - C.y;
    vec2 uv = (C/camera.resolution)*2.0-1.0;

    vec2 noiseSize = vec2(512);
    ivec2 repCoord = ivec2(fract(C/noiseSize) * noiseSize);
    float tang = mod(float(camera.frameIndex), PI*2.);
    vec2 repOffset = vec2(cos(tang), sin(tang)) * 10.;
    vec4 jit4 = texelFetch(blueNoiseTex,
        ivec2( fract((vec2(repCoord)+repOffset)/noiseSize) * noiseSize ),
    0).xyzw;
    // float jit = (jit4.x+jit4.y+jit4.z+jit4.w)/4.;
    float jit = jit4[camera.frameIndex % 4];

    vec3 eye = rel_position;
	vec4 tn = camera.viewProjInv * vec4(uv, 1.0, 1.0);
	vec3 dir = normalize(tn.xyz/tn.w);

    dir = mulvq(inv_rotquat, dir);
    eye = mulvq(inv_rotquat, eye);

    float radius = 1.0;

    //https://www.volumeofcube.com/circumscribed-sphere-radius
    // float circum = radius * 1.73205081;
    // float leye = length(eye);
    // if (leye > circum){
    //     eye += dir * (leye - circum);
    // }

    vec4 acc = vec4(0);

    vec3 normal;
    bool inside = false;
    vec2 tfn;
    bool hit = false;
    float dp = isphere(eye, dir, radius, inside, hit, tfn);
    // float dp = itorus(eye.xzy, dir.xzy, vec2(0.5), inside, hit, tfn);
    // acc.xyz += nTorus((eye + dir * dp).xzy, vec2(0.5)) *.5+.5;
    if (!hit){
        // FragColor = vec4(vPosition, 1);
        // return;
        discard;
    }

    vec3 sp = eye + dir * dp;
    if (inside){ sp = eye; }

    float eyedist = length(rel_position);
    float factor = 1.0;//max(0.0, eyedist/radius - 1.0) * 0.5 + 1.0;
    // acc += abs(tfn.x-tfn.y) * max(0.0, eyedist/(radius*1.5) - 1.0) * 0.025;
    acc += getMatterDensity(sp, dir, radius, eye, jit);

    FragColor = vec4(acc);
}

#endif
