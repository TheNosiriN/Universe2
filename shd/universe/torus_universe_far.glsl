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

layout(location = 0) c_attrib vec3 vPosition;
layout(location = 1) c_attrib vec3 true_position;
layout(location = 2) c_attrib flat uint object_index;




#ifdef HX_VERTEX_SHADER

layout (location = 0) in vec3 pos;

void main(){
    object_index = gl_InstanceIndex;
    SpaceObjectGPU obj = space_objects[object_index];

    vPosition = pos.xyz;
    vec3 finalpos = mulvq(obj.rotation, pos.xyz) - obj.position.xyz;

    vec4 renderpos = camera.projection * camera.view * vec4(finalpos, 1.0);
    gl_Position = renderpos;

    // float radius = obj.true_radius*2.0;
    // true_position = radius * (pos.xyz - obj.rel_pos_div_radius); // vertex_pos * true_radius * 2.0 - rel_position_in_cpp
    // true_position *= 8388608.0; // 2^23 (32 bit float mantissa bits)
    // true_position *= (1.175494e-38); // 2^23 (32 bit float mantissa bits)
}

#endif






#ifdef HX_FRAGMENT_SHADER

layout(location = 0) out vec4 FragColor;



float worley(vec3 P, float scale)
{
    float dist = 1.0;
    P *= scale;
    vec3 I = floor(P);
    vec3 F = fract(P);

    for(int X = -1; X<=1; X++){
        for(int Y = -1; Y<=1; Y++){
            for(int Z = -1; Z<=1; Z++){
                vec3 np = I + vec3(X,Y,Z);
                np = mod(abs(np), scale)*sign(np);
                float d = dot2((hash33(np) + vec3(X,Y,Z)) - F);
                dist = min(dist, d);
            }
        }
    }

    return sqrt(dist);
}

float fbm(vec3 p, float scale){
    float steps = 4.0;
    float sm = 0.0, mx = 0.0;
    float f = 1.0;
    float a = 1.0;
    for (int i=0; i<int(steps); ++i){
        sm += worley(p, scale*f) * a;
        mx += a;

        f *= 2.0;
        a *= 0.5;
    }
    return sm / mx;
    /*p *= 10.0;
    return worley(p*.06125)*.5 +
          worley(p*.125)*.25 +
          worley(p*.25)*.125 +
          worley(p*.5)*.06125;*/
}


float makeCol(vec2 uv){
    vec2 guv = uv*2.0-1.0;
    float col1 = easeInExpoFac(max(pow(guv.y, 7.0), 0.0), 1.5);
    float col2 = easeInExpoFac(max(pow(-guv.y, 5.0), 0.0), 1.7);
    float col = col1 + col2;
    col *= 1.0-easeInCirc(min(1.3-abs(guv.y+0.2), 1.0));
    return col;
}

vec2 getTorusUV(vec3 p, vec2 torus){
    return vec2(
        atan(p.z, p.x),
        atan(p.y, length(p.xz) - torus.x)
    ) / PI * 0.5 + 0.5;
}

void main(){

    vec2 uv = getTorusUV(vPosition, vec2(0.5));
    float col = makeCol(uv) * 10.0;

    FragColor = vec4(vec3(col)*col, col);
    // FragColor = vec4(vPosition*.5+.5, 1);
    // gl_FragDepth = 1.0/length(true_position);
}

#endif
