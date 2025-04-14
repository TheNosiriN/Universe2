#version 460
#shadertype VERTEX_SHADER
#shadertype FRAGMENT_SHADER

#include "common.inl.glsl"
#include "shader_globals.inl.glsl"


layout(binding = 0) uniform CameraData {
    CameraConstants camera;
};
layout(binding = 1) uniform PointLights {
    PointLightSpriteObjGPU point_lights[MAX_UBO_PLS_COUNT];
};

layout(location = 0) c_attrib vec2 vertpos;
layout(location = 1) c_attrib flat uint object_index;




#ifdef HX_VERTEX_SHADER

layout (location = 0) in vec3 pos;

void main(){
    object_index = gl_InstanceIndex;
    PointLightSpriteObjGPU obj = point_lights[object_index];

	vec3 camUp = vec3(
		camera.view[0][1], camera.view[1][1], camera.view[2][1]
	);
	vec3 camRight = vec3(
		camera.view[0][0], camera.view[1][0], camera.view[2][0]
	);

    float scale = obj.scale;
	vertpos = triangle_strip_quad_vertices[ gl_VertexIndex ];
	vec3 rasterpos = camRight*vertpos.x*scale + camUp*vertpos.y*scale;
    rasterpos -= obj.position.xyz;
	gl_Position = camera.projection * camera.view * vec4(rasterpos, 1);

    // vec4 renderpos = camera.projection * camera.view * vec4(finalpos, 1.0);
    // gl_Position = renderpos;

}

#endif






#ifdef HX_FRAGMENT_SHADER

layout(location = 0) out vec4 FragColor;

void main(){
    float col = 0.05/max(0.0, length(vertpos)-0.1);//0.01/max(0., length(vertpos)-0.05);
    col = smoothstep(0.0, 5.0, col) * 50.0;
    FragColor = vec4(col);// + vec4(vertpos*.5+.5, 0, 1);
    // gl_FragDepth = 1.0/length(true_position);
}

#endif
