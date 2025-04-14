#version 460
#shadertype COMPUTE_SHADER

#include "common.inl.glsl"
#include "shader_globals.inl.glsl"
#include "cosmicweb_func.inl.glsl"


layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;
layout (binding = 0, r8) uniform image3D outputImage;


void main(){
    ivec3 Ci = ivec3(gl_GlobalInvocationID.xyz);
    vec3 C = vec3(Ci);
    float R = float(COSMICWEB_DENSITY_VIEW_CELL_SIZE);
    if (C.x>=R || C.y>=R || C.z>=R) { return; }

    vec3 uvw = C/R * 2.0 - 1.0;
    float val = cosmicWebDensity(uvw, 1.0);

    imageStore(outputImage, Ci, vec4(val));
}
