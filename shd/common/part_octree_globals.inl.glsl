#ifndef __CONVERT_SHADER_HEADER
	#define c_out_param(x) out x
	#define alignas(x)
	#define static
#else
	#define c_out_param(x) x&
#endif



struct ParticleOctreeNode {
    vec3 position;
    // float value;
    // vec3 __padding0;
    float radius;
};

struct ParticleOctreeProps {
    vec3 gridSize;
    float maxDepth;
    vec3 position;
    float minGridSize;
    vec4 rotation;
    vec4 rotationInv;
};
