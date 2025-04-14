#version 460
#shadertype VERTEX_SHADER
#shadertype FRAGMENT_SHADER
#extension GL_EXT_nonuniform_qualifier : enable

#include "common.inl.glsl"
#include "shader_globals.inl.glsl"


layout(binding = 0) uniform sampler2D InputTexture[3];

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

vec3 ACESFilm(vec3 x){
    float a = 2.51;
    float b = 0.03; float c = 2.43;
    float d = 0.59; float e = 0.14;
    return (x*(a*x+b))/(x*(c*x+d)+e);
}

void main(){
    // FragColor = vec4(ACESFilm(texture(InputTexture[0], vUV).xyz), 1);

    vec4 solid = texture(InputTexture[0], vUV);
    vec4 transparent = texture(InputTexture[1], vUV);
    // scene = 1.0 - exp(-acc * exposure);
    vec3 final = mix(solid.rgb, transparent.rgb, transparent.a);

    vec4 text = texture(InputTexture[2], vUV);
    final = mix(final, text.rgb, text.a);

    FragColor = vec4(final, 1);
}
#endif
