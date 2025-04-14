#ifndef __CONVERT_SHADER_HEADER
	#define c_out_param(x) out x
	#define alignas(x)
	#define static
#else
	#define c_out_param(x) x&
#endif


#define FONT_MAX_CHARS 256
#define FONT_MAX_GLYPHS 256


struct GlyphsGPU {
    vec4 atlasBounds;
};

struct RenderTextChar {
    vec2 position;
    vec2 scale;
    vec3 props;
    uint index;
};

struct FontWindowConstants {
    vec2 screenPxRangeScale;
    float screenRatio;
    float distanceRange;
};
