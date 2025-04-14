#ifndef __CONVERT_SHADER_HEADER
	#define c_out_param(x) out x
	#define alignas(x)
	#define static
#else
	#define c_out_param(x) x&
#endif


#define MAX_UBO_SPC_OBJ_STRUCT_COUNT 512
#define COSMICWEB_DENSITY_VIEW_CELL_SIZE 64
#define MAX_UBO_PLS_COUNT 256


struct WindowConstants {
    vec2 resolution;
    float FrameIndex;
    float __padding0;
};

struct CameraConstants {
	mat4 projection;
	mat4 view;
    mat4 viewProjInv;
    vec2 resolution;
    uint frameIndex;
    float exposure;
};

struct MultiverseWorldConstants {
    float up_time;
};

struct SpaceObjectGPU {
    vec3 position;
    float scale;
    vec4 rotation;
    vec4 rotationInv;
    vec3 rel_pos_div_radius;
    float true_radius;
};

struct PointLightSpriteObjGPU {
    vec3 position;
    float scale;
};

struct InactiveSpaceObjectGPU {
    vec3 position;
    float value;
    vec4 rotation;
    vec4 rotationInv;
};
