#version 460
#shadertype VERTEX_SHADER
#shadertype FRAGMENT_SHADER

#include "common.inl.glsl"
#include "shader_globals.inl.glsl"


layout(binding = 0) uniform CameraData {
    CameraConstants camera;
};

layout(push_constant) uniform ParentProps {
    InactiveSpaceObjectGPU parent;
};

layout(location = 0) c_attrib float vValue;




#ifdef HX_VERTEX_SHADER

layout(location = 0) in float value;


void main() {
    vec3 localpos = (vec3(
        ind_1Dto3D(gl_VertexIndex, ivec3(COSMICWEB_DENSITY_VIEW_CELL_SIZE))
    ) / float(COSMICWEB_DENSITY_VIEW_CELL_SIZE)) * 2.0 - 1.0;
    vValue = value;

    vec3 finalpos = mulvq(parent.rotation, -localpos) - parent.position.xyz;
    gl_Position = camera.projection * camera.view * vec4(finalpos, 1.0);
    gl_PointSize = 1000.0;
}

#endif






#ifdef HX_FRAGMENT_SHADER

layout(location = 0) out vec4 FragColor;


void main(){
    FragColor = vec4(vValue);
}

#endif
