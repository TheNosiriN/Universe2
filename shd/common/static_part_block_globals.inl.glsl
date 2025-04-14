#ifndef __CONVERT_SHADER_HEADER
	#define c_out_param(x) out x
	#define alignas(x)
	#define static
#else
	#define c_out_param(x) x&
#endif


#define MAX_STATIC_PARTICLES_BLOCK_DEPTH 16


struct StaticParticlesBlockProps {
    vec4 rotation;
    vec4 rotationInv;
    vec2 __padding0;
    uint gridSize;
    uint levels;
};
