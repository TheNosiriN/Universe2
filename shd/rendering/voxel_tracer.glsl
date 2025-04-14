#version 460
#shadertype VERTEX_SHADER
#shadertype FRAGMENT_SHADER

#include "common.inl.glsl"
#include "shader_globals.inl.glsl"

const float voxelSize = 0.05;
const vec3 cubeRadius = vec3(1,2,1);


layout(binding = 1) uniform CameraData {
    CameraConstants camera;
};

layout(location = 0) c_attrib vec3 vPosition;
layout(location = 1) c_attrib flat int maxsteps;


#ifdef HX_VERTEX_SHADER

layout (location = 0) in vec3 pos;

void main(){
    int tri = gl_VertexIndex / 3;
    int idx = gl_VertexIndex % 3;
    int face = tri / 2;
    int top = tri % 2;

    int dir = face % 3;
    int pos = face / 3;

    int nz = dir >> 1;
    int ny = dir & 1;
    int nx = 1 ^ (ny | nz);

    vec3 d = vec3(nx, ny, nz);
    float flip = 1 - 2 * pos;

    vec3 n = flip * d;
    vec3 u = -d.yzx;
    vec3 v = flip * d.zxy;

    float mirror = -1 + 2 * top;
    vec3 xyz = n + mirror*(1-2*(idx&1))*u + mirror*(1-2*(idx>>1))*v;
    xyz *= cubeRadius;

    vPosition = xyz;
    maxsteps = int((max3(cubeRadius) * 1.73205081) / voxelSize)*4 + 1;

    gl_Position = camera.projection * camera.view * vec4(xyz, 1.0);
}
#endif



#ifdef HX_FRAGMENT_SHADER

layout(location = 0) out vec4 FragColor;


vec3 worldToVoxel(vec3 i){
    return floor(i/voxelSize);
}
vec3 voxelToWorld(vec3 i){
    return i*voxelSize;
}

float scene(vec3 p){
    return min(-p.y+0.25, length(p)-0.8);
}
float cube(vec3 p, vec3 rad){
    vec3 q = abs(p) - rad;
    return max3(q);
}

float voxelTrace(vec3 ro, vec3 rd, int maxsteps, out bool hit, out vec3 hitNormal, out vec3 hitVoxel){
    const float isoValue = 0.0;
    vec3 voxel = worldToVoxel(ro);
    vec3 dstep = sign(rd);

    vec3 nearestVoxel = voxel + vec3(rd.x > 0.0, rd.y > 0.0, rd.z > 0.0);
    vec3 tMax = (voxelToWorld(nearestVoxel) - ro) / rd;
    vec3 tDelta = voxelSize / abs(rd);
    hitVoxel = voxel;

    vec3 mask;
    float hitT = 0.0;
    hit = false;
    for(int i=0; i<maxsteps; i++) {
        vec3 p = voxelToWorld(voxel);
        // if (cube(p, cubeRadius+voxelSize) < 0.0){
        //     break;
        // }
        float d = max(scene(p), cube(p, cubeRadius));
        if (d <= 0.0) {
            hit = true;
	    	hitVoxel = voxel;
            break;
        }

        // mask = vec3(lessThanEqual(tMax.xyz, min(tMax.yzx, tMax.zxy)));
        mask = step(tMax.xyz, tMax.yzx) * step(tMax.xyz, tMax.zxy);
        voxel += dstep * mask;
        tMax += tDelta * mask;
    }

    hitNormal = -dstep * mask;
    hitT = length(mask * tMax);
    return hitT;
}



void main(){
    //https://www.volumeofcube.com/circumscribed-sphere-radius
    float circum = max3(cubeRadius) * 1.73205081;

    vec3 eye = -camera.eye;
    vec3 dir = normalize(camera.eye - vPosition);

    float leye = length(eye);
    if (leye > circum){
        eye += dir * (leye - circum);
    }
    // eye = -cam;

    bool hit;
    vec3 voxel, normal;
    float dist = voxelTrace(eye, dir, maxsteps, hit, normal, voxel);
    if (!hit){ discard; }

    vec3 color = normalize(voxelToWorld(voxel));

    FragColor = vec4(color,1);
}

#endif
