#version 460
#shadertype VERTEX_SHADER
#shadertype FRAGMENT_SHADER
#extension GL_EXT_nonuniform_qualifier : enable

#include "common.inl.glsl"
#include "shader_globals.inl.glsl"


layout(binding = 0) uniform sampler2D ScreenTexture;

layout(location = 0) c_attrib vec2 vUV;


#ifdef HX_VERTEX_SHADER

layout (location = 0) in vec3 pos;

void main(){
    vUV = vec2(uvec2(gl_VertexIndex, gl_VertexIndex << 1) & 2);
    gl_Position = vec4(mix(vec2(-1, 1), vec2(1, -1), vUV), 0, 1);
}
#endif



#ifdef HX_FRAGMENT_SHADER

layout(location = 0) out vec4 FragColor;

void main(){
    FragColor = texture(ScreenTexture, vUV);
}
#endif
