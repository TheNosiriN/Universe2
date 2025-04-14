#version 460
#shadertype VERTEX_SHADER
#shadertype FRAGMENT_SHADER

#include "common.inl.glsl"
#include "shader_globals.inl.glsl"


layout(binding = 0) uniform CameraData {
    CameraConstants camera;
};
layout(binding = 1) uniform SpaceObjects {
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
    vec3 finalpos = pos.xyz*obj.scale - obj.position.xyz;

    vec4 renderpos = camera.projection * camera.view * vec4(finalpos, 1.0);
    gl_Position = renderpos;

    float radius = obj.true_radius*2.0;
    true_position = radius * (pos.xyz - obj.rel_pos_div_radius); // vertex_pos * true_radius * 2.0 - rel_position_in_cpp
    true_position *= 8388608.0; // 2^23 (32 bit float mantissa bits)
}

#endif






#ifdef HX_FRAGMENT_SHADER

layout(location = 0) out vec4 FragColor;

#define dot2(x) dot(x,x)

void main(){
    FragColor = vec4(vPosition, uintBitsToFloat(object_index));
    gl_FragDepth = 1.0/length(true_position);

    // FragColor.xyz = vec3(gl_FragDepth);
}

#endif
