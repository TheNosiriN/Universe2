#version 460
#shadertype VERTEX_SHADER
#shadertype FRAGMENT_SHADER

#include "common.inl.glsl"
#include "font_globals.inl.glsl"


layout(push_constant) uniform WindowConsts {
    FontWindowConstants window;
};
layout(binding = 0) uniform GlyphsBuffer {
    GlyphsGPU glyphs[FONT_MAX_GLYPHS];
};
layout(binding = 1) uniform TextBuffer {
    RenderTextChar chars[FONT_MAX_CHARS];
};

layout(binding = 2) uniform sampler2D atlasTexture;

layout(location = 0) c_attrib flat vec4 atlasBounds;
layout(location = 1) c_attrib vec2 vertpos;
layout(location = 2) c_attrib float screenPxRange;




#ifdef HX_VERTEX_SHADER

layout (location = 0) in vec3 pos;

float getScreenPxRange(float quadSize){
    const float distanceRange = 4.0;
    const float charSize = 64.0;
    return quadSize * distanceRange/charSize;
}

void main(){
    RenderTextChar c = chars[gl_InstanceIndex];
    GlyphsGPU glyph = glyphs[c.index];
    atlasBounds = glyph.atlasBounds;
    if (atlasBounds.w == 0.0 && atlasBounds.y == 0.0)return;

    vec2 total_scale = c.scale;
    vec2 pxscale = total_scale * window.screenPxRangeScale;
    screenPxRange = min(pxscale.x, pxscale.y) * window.distanceRange;//getScreenPxRange(min(pxscale.x, pxscale.y));

    vertpos = triangle_strip_quad_vertices[gl_VertexIndex] * 0.5 + 0.5;
    vec2 rasterpos = c.position + vertpos*total_scale;
	gl_Position = vec4(rasterpos * vec2(1.0/window.screenRatio, 1.0), 0, 1);
}

#endif






#ifdef HX_FRAGMENT_SHADER

layout(location = 0) out vec4 FragColor;

float median3(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

float median4(float r, float g, float b, float a) {
    return (r + g + b + a - max(max(r, g), max(b, a)) - min(min(r, g), min(b, a))) / 2.0;
}

void main(){
    vec2 scale = vec2(abs(atlasBounds.w-atlasBounds.y), abs(atlasBounds.z-atlasBounds.x));
    if (scale.x == 0.0)discard;

    vec2 uv = vec2(vertpos.x, vertpos.y) * scale;
    uv += vec2(atlasBounds.w, atlasBounds.z); // vec2(bottom, left)

    vec4 msd = texture(atlasTexture, vec2(uv.x, 1.0-uv.y), 0).rgba;
    float sd = median4(msd.r, msd.g, msd.b, msd.a);

    float screenPxDistance = screenPxRange * (sd - 0.5);
    float alpha = saturate(screenPxDistance + 0.5);//clamp(screenPxDistance + 0.5, 0.0, 1.0);

    // float thickness = 0.5 * screenPxRange;
    // float softness = 0.5 * screenPxRange;
    // float alpha = smoothstep(1.0 - thickness - softness, 1.0 - thickness + softness, sd);

    vec3 color = vec3(alpha);
    FragColor = vec4(color, sd);
}

#endif
