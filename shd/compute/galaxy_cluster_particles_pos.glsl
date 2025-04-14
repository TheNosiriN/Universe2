#version 460
#shadertype COMPUTE_SHADER

#include "common.inl.glsl"
#include "shader_globals.inl.glsl"

layout (local_size_x = 8, local_size_y = 8, local_size_z = 8) in;
layout (binding = 0, r8) uniform image3D cosmicWebTex;




float perlin(vec3 p){
    vec3 pi = floor(p);
    vec3 pf = p - pi;
    vec3 w = pf * pf * (3.0 - 2.0 * pf);

    return mix(
        mix(
        	mix(
                dot( pf - vec3(0, 0, 0), hash33(pi + vec3(0, 0, 0)) *2.0-1.0 ),
                dot( pf - vec3(1, 0, 0), hash33(pi + vec3(1, 0, 0)) *2.0-1.0 ),
            w.x),
        	mix(dot( pf - vec3(0, 0, 1), hash33(pi + vec3(0, 0, 1)) *2.0-1.0 ),
                dot( pf - vec3(1, 0, 1), hash33(pi + vec3(1, 0, 1)) *2.0-1.0 ),
            w.x),
        w.z),
        mix(
            mix(
                dot( pf - vec3(0, 1, 0), hash33(pi + vec3(0, 1, 0)) *2.0-1.0 ),
                dot( pf - vec3(1, 1, 0), hash33(pi + vec3(1, 1, 0)) *2.0-1.0 ),
            w.x),
           	mix(
                dot( pf - vec3(0, 1, 1), hash33(pi + vec3(0, 1, 1)) *2.0-1.0 ),
                dot( pf - vec3(1, 1, 1), hash33(pi + vec3(1, 1, 1)) *2.0-1.0 ),
            w.x),
        w.z),
    w.y) * 0.5 + 0.5;
}


float worley(vec3 P, float scale){
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


vec3 rndUnitSphere(float u, float v) {
    float theta = u * 2.0 * 3.14159265358979323846; // Azimuthal angle in [0, 2*PI]
    float phi = acos(2.0 * v - 1.0); // Polar angle in [0, PI]

    float sinTheta = sin(theta);
    float cosTheta = cos(theta);
    float sinPhi = sin(phi);
    float cosPhi = cos(phi);

    return vec3(
        sinPhi * cosTheta,
        sinPhi * sinTheta,
        cosPhi
    );
}


float fbm(vec3 p, float scale){
    float steps = 10.0;
    float sm = 0.0, mx = 0.0;
    float f = scale;
    float a = 1.0;
    for (int i=0; i<int(steps); ++i){
        float noise_u = perlin(p.yxz * f)*a;
        float noise_v = perlin(p.zyx * f)*a;
        p += rndUnitSphere(noise_u, noise_v) * 0.03;

        float samp = worley(p.xyz, f);
        sm += samp * a;
        mx += a;

        f *= 2.0;
        a *= 0.5;
    }

    return sm / mx;
}



void main(){
    ivec3 Ci = ivec3(gl_GlobalInvocationID.xyz);
    vec3 C = vec3(Ci);
    vec3 R = imageSize(cosmicWebTex);
    if (C.x>=R.x || C.y>=R.y || C.z>=R.z) { return; }

    vec3 uvw = (C/R) * 2.0 - 1.0;

    float worley = fbm(uvw, 2.5);
    worley = smoothstep(0.0, 1.0, worley);
    float tch = 1.3;
    worley = min(1.0, easeInExpoFac(worley, tch) * tch);
    // worley *= smoothstep(0.25, 0.5, worley);
    float density = 1.0-worley;

    imageStore(cosmicWebTex, Ci, vec4(density));
}
