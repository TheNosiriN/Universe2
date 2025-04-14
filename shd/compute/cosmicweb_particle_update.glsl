#version 460
#shadertype COMPUTE_SHADER

#include "common.inl.glsl"
#include "shader_globals.inl.glsl"
#include "cosmicweb_func.inl.glsl"

layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;
layout(binding = 0) buffer ParticlesBuffer {
    vec4 particles[];
};



vec3 hash31(uint q){
	uvec3 n = q * uvec3(1597334673U, 3812015801U, 2798796415U);
	n = (n.x ^ n.y ^ n.z) * uvec3(1597334673U, 3812015801U, 2798796415U);
	return vec3(n) * 2.328306437080797e-10;
}


float getsdf(vec3 p){
    float d = 0.6-cosmicWebDensity(p, 0.5);
    d = max(max3(abs(p)-1.0), d) * 0.3;
    // return smoothstep(0.0, 1.0, cosmicWebDensity(p, 0.5));
    return d;
}

vec3 getnormal(vec3 p){
    // float eps = 0.001;
    // return normalize(vec3(
    //     getsdf(p+vec3(eps,0,0)) - getsdf(p-vec3(eps,0,0)),
    //     getsdf(p+vec3(0,eps,0)) - getsdf(p-vec3(0,eps,0)),
    //     getsdf(p+vec3(0,0,eps)) - getsdf(p-vec3(0,0,eps))
    // ));

    vec3 ep = vec3(-4, 4, 0) * 0.001;
    return normalize(
        getsdf(p+ep.xyy) * ep.xyy +
        getsdf(p+ep.yxy) * ep.yxy +
        getsdf(p+ep.yyx) * ep.yyx +
        getsdf(p+ep.xxx) * ep.xxx
    );
}

void main(){
    uint C = uint(gl_GlobalInvocationID.x);
    if (C >= MAX_COSMICWEB_PARTICLES_COUNT)return;

    vec3 pos = hash31(C) * 2.0 - 1.0;
    float sdf = getsdf(pos);
    vec3 norm = getnormal(pos);
    pos -= norm * sdf;

    float density = sdf;
    particles[C] = vec4(pos, density);
}
