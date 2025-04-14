#version 460
#shadertype VERTEX_SHADER
#shadertype FRAGMENT_SHADER

#include "common.inl.glsl"
#include "shader_globals.inl.glsl"
#include "static_part_block_globals.inl.glsl"
#include "cosmicweb_func.inl.glsl"


layout(binding = 0) uniform CameraData {
    CameraConstants camera;
};

layout(binding = 1) uniform RelPosBuffer {
    vec4 positions[MAX_STATIC_PARTICLES_BLOCK_DEPTH];
};

layout(binding = 2) uniform sampler3D cosmicWebDenityTex;

layout(push_constant) uniform Props {
    StaticParticlesBlockProps props;
};

layout(location = 0) c_attrib float minlight;
layout(location = 1) c_attrib vec2 vertpos;




#ifdef HX_VERTEX_SHADER

layout (location = 0) in vec3 inpos;


bool isPointOnScreen(vec4 rasterpos) {
    // Transform the point to clip space
    // vec4 rasterpos = viewProjectionMatrix * vec4(point, 1.0);

    // Perform the homogeneous divide to get normalized device coordinates (NDC)
    vec3 ndcPoint = rasterpos.xyz / rasterpos.w;

    // Check if the point is within the clip space boundaries
    // Clip space boundaries are between -1 and 1 for x, y, and z
    return ndcPoint.x >= -1.0 && ndcPoint.x <= 1.0 &&
           ndcPoint.y >= -1.0 && ndcPoint.y <= 1.0 &&
           ndcPoint.z >= -1.0 && ndcPoint.z <= 1.0;
}


bool evaluate2(vec3 p) {
    // float d = length(p) - 0.5;
    // return d;//max(d, 0.0);

    float noise = cosmicWebDensity(p, 1.0);
    // float noise = texture(cosmicWebDenityTex, p*0.5+0.5).r;
    noise = smoothstep(0.5, 1.0, noise) + noise;
    noise = smoothstep(0.2, 1.0, noise);
    return noise > 0.25;
}


void discard_vertex(){
    gl_CullDistance[0] = -1.0;
}


void main(){
    const uint total = props.gridSize*props.gridSize*props.gridSize;
    int index = int(gl_InstanceIndex) % int(total);
    uint level = uint(gl_InstanceIndex) / total;
    vec4 position = positions[level];

    float gridscale = 1.0/float(props.gridSize);
    float scale = 0.1;
    vec3 vpos = vec3(ind_1Dto3D(index, ivec3(props.gridSize)));
    vec3 localpos = vpos*gridscale;
    vec3 localcellpos = (localpos * 2.0 - 1.0) * position.w;
    localpos = (localpos + hash33(localcellpos)*abs(scale-gridscale)) * 2.0 - 1.0;

    vec3 globalpos = position.xyz;
    vec3 abspos = globalpos.xyz + localpos;//mulvq(props.rotation, localpos);

    vec3 camUp = vec3(
		camera.view[0][1], camera.view[1][1], camera.view[2][1]
	);
	vec3 camRight = vec3(
		camera.view[0][0], camera.view[1][0], camera.view[2][0]
	);
    mat4 viewProj = camera.projection * camera.view;

    // culling
    bool onscreen = false;
    for (int i=0; i<3; ++i){
        vec2 tvertpos = (triangle_vertices[i]*2.0) * 2.0 - 1.0;
    	vec3 trenderpos = (camRight*tvertpos.x*gridscale*scale + camUp*tvertpos.y*gridscale*scale) - abspos;
        vec4 rasterpos = viewProj * vec4(trenderpos, 1);
        if (isPointOnScreen(rasterpos)){
            onscreen = true;
            break;
        }
    }

    if (!onscreen){
        discard_vertex();
        return;
    }

    if (!evaluate2(localcellpos)){
        discard_vertex();
        return;
    }

    // minlight = 1.0 - (levelscale);
    vertpos = (triangle_vertices[gl_VertexIndex]*2.0) * 2.0 - 1.0;
	vec3 renderpos = (camRight*vertpos.x*gridscale*scale + camUp*vertpos.y*gridscale*scale) - abspos;
	gl_Position = viewProj * vec4(renderpos, 1);
}

#endif






#ifdef HX_FRAGMENT_SHADER

layout(location = 0) out vec4 FragColor;


void main(){
    float col = min(0.6, 0.5/max(0.0, length(vertpos)+0.5));
    col = smoothstep(0.45, 0.7, col);
    if (col <= 0.01)discard;
    FragColor = vec4(col);// + vec4(vertpos*(1.0-col), 0, (1.0-col));
}

#endif
