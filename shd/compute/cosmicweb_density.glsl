#version 460
#shadertype COMPUTE_SHADER

#include "common.inl.glsl"
#include "shader_globals.inl.glsl"
#include "cosmicweb_func.inl.glsl"


#define USE_CUBE


layout(local_size_x = 32, local_size_y = 32) in;

layout(push_constant) uniform SpaceObject {
    InactiveSpaceObjectGPU obj;
};
layout(binding = 0) uniform CameraData {
    CameraConstants camera;
};
layout(binding = 1) uniform sampler3D cosmicWebDenityTex;
layout(binding = 2) uniform sampler2D blueNoiseTex;
layout(binding = 3, rgba8) uniform image2D outputImage;



vec3 cosmicWebColor(float value) {
    vec3 darkPurple = vec3(0.2, 0.0, 0.4);
    vec3 purple = vec3(0.5, 0.0, 0.5);
    vec3 magenta = vec3(0.8, 0.0, 0.4);
    vec3 orange = vec3(1.0, 0.5, 0.1);
    vec3 yellow = vec3(1.0, 1.0, 0.1);

    // Calculate the interpolation factor
    float v = value * 4.0;

    // Blend colors based on value
    vec3 c = (v < 1.0) ? mix(darkPurple, purple, v) :
             (v < 2.0) ? mix(purple, magenta, v - 1.0) :
             (v < 3.0) ? mix(magenta, orange, v - 2.0) :
                         mix(orange, yellow, v - 3.0);

    // Apply the square root adjustment
    return c * sqrt(value);
}


vec4 textureNice3D(sampler3D sam, vec3 uvw){
    float textureResolution = float(textureSize(sam,0).x);
    uvw = uvw*textureResolution + 0.5;
    vec3 iuvw = floor( uvw );
    vec3 fuvw = fract( uvw );
    uvw = iuvw + fuvw*fuvw*(3.0-2.0*fuvw);
    uvw = (uvw - 0.5)/textureResolution;
    return texture( sam, uvw );
}

float getdensity(vec3 p){
    // float noise = cosmicWebDensity(p, 2.0);
    float noise = texture(cosmicWebDenityTex, p*0.5+0.5).r;
    noise = smoothstep(0.5, 1.0, noise) + noise;
    noise = smoothstep(0.2, 1.0, noise);
    return noise;
}

vec4 raymarch(vec3 sp, vec3 dir, float radius, int steps, float stepsize, float jit){
    vec4 acc = vec4(0);
    float dist = 0.0;
    for (int i=0; i<steps; ++i){

        vec3 cp = sp + dir * dist;
    #ifdef USE_CUBE
        float nd = max3(abs(cp)-radius);
    #else
        float nd = length(cp) - radius;
    #endif
        if(nd >= 0.001)break;

        float density = getdensity(cp);
        vec4 col = vec4(cosmicWebColor(density), density*0.5);

        acc.xyz += col.xyz*col.a * (1.0-acc.a);
        acc.a += col.a;
        acc.a = min(acc.a, 1.0);
        if (acc.a >= 0.99)break;

        float fac = (1.0-pow(min(1.0, dist/radius), 1.0)) * 0.05;
        dist += stepsize*max(0.05, dist) + jit*fac;
    }

    acc.xyz *= 2.0;
    return acc;
}


void main(){
    ivec2 Ci = ivec2(gl_GlobalInvocationID.xy);
    vec2 C = vec2(Ci);
    vec2 R = imageSize(outputImage);
    vec2 uv = (C/R) * 2.0 - 1.0;
    uv.y = -uv.y;
    if (Ci.x>=R.x && Ci.y>=R.y){ return; }

	vec3 eye = obj.position;
	vec3 rasterpos = vec3(uv,1);
	vec3 tn = (camera.viewProjInv * vec4(rasterpos,1)).xyz;
	vec3 dir = normalize(tn);

	dir = mulvq(obj.rotationInv, dir);
	eye = mulvq(obj.rotationInv, eye);


    vec4 color = vec4(0);

    float radius = 1.0;

    vec3 normal;
    bool inside;
    vec2 tfn;
    bool hit;
#ifdef USE_CUBE
    float dp = ibox(eye, dir, vec2(0.001, 10000.0), vec3(radius), inside, hit, tfn);
#else
    float dp = isphere(eye, dir, radius, inside, hit, tfn);
#endif

    if (!hit){
        imageStore(outputImage, Ci, vec4(0));
        return;
    }

    int steps = 32;
    float stepsize = (radius*2.0)/float(steps);

    vec3 sp = eye;
    if (!inside){
        sp = eye + dir * dp;
    }

    vec2 noiseSize = vec2(textureSize(blueNoiseTex, 0).xy);
    ivec2 repCoord = ivec2(fract(C/noiseSize) * noiseSize);
    float tang = mod(float(camera.frameIndex), 3.142*2.);
    vec2 repOffset = vec2(cos(tang), sin(tang)) * 10.;
    vec4 jit4 = texelFetch(blueNoiseTex,
        ivec2(fract( (vec2(repCoord)+repOffset) /noiseSize) * noiseSize),
    0).xyzw;
    float jit = jit4[camera.frameIndex % 4];

    color = raymarch(sp, dir, radius, steps, stepsize, jit);
    imageStore(outputImage, Ci, color);

}
