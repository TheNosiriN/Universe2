// #version 460
// #shadertype COMPUTE_SHADER
//
// #include "common.inl.glsl"
// #include "part_octree_globals.inl.glsl"
// #include "cosmicweb_func.inl.glsl"
//
//
// layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;
// layout(binding = 0, rgba8) uniform image2D voxelgalaxyTex;
//
// layout(push_constant) uniform Props {
//     ParticleOctreeProps props;
// };
//
//
// void main(){
//     ivec2 Ci = ivec3(gl_GlobalInvocationID.xy);
//     int depth = int(gl_GlobalInvocationID.z);
//     vec2 C = vec3(Ci);
//     vec2 R = imageSize(voxelgalaxyTex);
//     if (C.x>=R.x || C.y>=R.y) { return; }
//
//
// }











#version 460
#shadertype COMPUTE_SHADER

#include "common.inl.glsl"
#include "part_octree_globals.inl.glsl"
#include "cosmicweb_func.inl.glsl"

#define _STACK_SIZE 128

layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;


layout(binding = 0) buffer LeafNodes {
    ParticleOctreeNode leafNodes[];
};
layout(binding = 1) buffer LeafCounter {
    uint leafCounter;
};
layout(push_constant) uniform Props {
    ParticleOctreeProps props;
};



float evaluate(vec3 p, float scale, float radius, float depth, float shape) {
    float density = easeInExpoFac(shape+0.25, 2.0)*4.0;
    float sizemask = step(hash13(p*scale), density);
    float value = sizemask;
    return value;
}

float evaluate2(vec3 p, float depth) {
    // float d = length(p) - 0.5;
    // return max(-d, 0.0);

    float d = cosmicWebDensitySimple(p, 1.0) - 0.25;
    // d *= step(length(p), 1.0);
    return max(d, 0.0);
}



struct StackEntry {
    vec3 position;
    float depth;
    float value;
};

StackEntry stack[_STACK_SIZE];
int stackPointer = 0;

void stack_push(StackEntry entry){
    stack[stackPointer] = entry;
    stackPointer++;
}

StackEntry stack_pop(){
    stackPointer--;
    return stack[stackPointer];
}



void add_leaf(vec3 position, float depth, float value){
    ParticleOctreeNode out_node;
    out_node.position = position;
    out_node.radius = depth;
    // out_node.value = value;
    // out_node.__padding0 = vec3(0);
    uint leaf_index = atomicAdd(leafCounter, 1);
    leafNodes[leaf_index] = out_node;
}


vec3 getnormpos(vec3 pos){
    return pos * 2.0 - 1.0;
}

void main() {
    vec3 gridpos = vec3(gl_GlobalInvocationID.xyz);
    if (any(greaterThanEqual(gridpos, props.gridSize)))return;

    StackEntry root;
    root.position = gridpos / props.minGridSize;
    root.depth = 0;
    float root_radius = 0.5 / props.minGridSize;

    vec3 center = getnormpos(root.position + root_radius);
    root.value = evaluate2(center, root.depth);
    stack_push(root);

    while (stackPointer > 0) {
        StackEntry node = stack_pop();
        float node_radius = root_radius / float(1u << uint(node.depth));
        float shape_average = node.value;

        StackEntry children[8];
        for (uint i=0; i<8; ++i){
            StackEntry child;
            child.position = node.position + vec3(i & 1u, i >> 1u & 1u, i >> 2u & 1u)*node_radius;
            child.depth = node.depth + 1;
            float child_radius = root_radius / float(1u << uint(child.depth));
            vec3 center = getnormpos(child.position + child_radius);
            child.value = evaluate2(center, node.depth);
            shape_average += child.value;
            children[i] = child;
        }

        shape_average /= 9;
        if (shape_average <= 0.0)continue;

        // run evaluate
        vec3 center = getnormpos(node.position + node_radius);
        float scale = props.minGridSize * float(1u << uint(node.depth));
        float centerval = evaluate(center, scale, node_radius, node.depth, shape_average);

        // if it was mean't to spawn at this depth then add to the leafs array
        if (centerval >= 1.0){
            center += (hash33(center*scale)*2.0-1.0) * node_radius;
            add_leaf(center, node_radius, centerval);
            continue;
        }

        // float observerdist = min(1.0, distance(center, props.position));
        float observerdist = min(1.0, distance(center, props.position));
        float maxDepth = props.maxDepth;//mix(props.maxDepth, 0, observerdist);
        // float maxDepth = easeInExpo(1.0-observerdist) * props.maxDepth;
        // float maxDepth = pow(1.0-observerdist, 2.0) * props.maxDepth;
        maxDepth = ceil(maxDepth);


        // if it's still 0 but reached the max depth, then it was never mean't to spawn
        if (node.depth >= maxDepth || stackPointer+8 >= _STACK_SIZE){
            // center += (hash33(center*scale)*2.-1.) * node.radius;
            // add_leaf(center, node.radius * 2.0, centerval);
            continue; // TODO: make this a gas particle
        }

        // add chilren
        for (uint i=0; i<8; ++i){
            stack_push(children[i]);
        }

    }

}





// void main() {
//     // vec3 gridpos = vec3(gl_GlobalInvocationID.xyz);
//     vec3 gridpos = ind_1Dto3D(int(gl_GlobalInvocationID), ivec3(props.gridSize));
//     if (any(greaterThanEqual(gridpos, props.gridSize)))return;
//
//     StackEntry root;
//     root.position = gridpos / props.minGridSize;
//     root.depth = 0;
//     float root_radius = 0.5 / props.minGridSize;
//
//     vec3 center = getnormpos(root.position + root_radius);
//     root.value = evaluate2(center, root.depth);
//
//     stack_push(root);
//
//
//     while (stackPointer > 0) {
//         StackEntry node = stack_pop();
//         StackEntry children[8];
//         float node_radius = root_radius / float(1 << uint(node.depth));
//         float shape_average = node.value;
//
//         for (uint i=0; i<8; ++i){
//             StackEntry child;
//             child.position = node.position + vec3(i & 1u, i >> 1u & 1u, i >> 2 & 1)*node_radius;
//             child.depth = node.depth + 1;
//             float child_radius = root_radius / float(1 << uint(child.depth));
//             vec3 center = getnormpos(child.position + child_radius);
//             child.value = evaluate2(center, node.depth);
//             shape_average += child.value;
//             children[i] = child;
//         }
//
//         shape_average /= 9;
//         if (shape_average <= 0.0)continue;
//
//         // run evaluate
//         vec3 center = getnormpos(node.position + node_radius);
//         float scale = props.minGridSize * float(1 << uint(node.depth));
//         float centerval = evaluate(center, scale, node_radius, node.depth, shape_average);
//
//         // if it was mean't to spawn at this depth then add to the leafs array
//         if (centerval >= 1.0){
//             center += (hash33(center*scale)*2.-1.) * node_radius;
//             add_leaf(center, node_radius, centerval);
//             continue;
//         }
//
//         // if it's still 0 but reached the max depth, then it was never mean't to spawn
//         if (node.depth >= props.maxDepth || stackPointer+8 >= _STACK_SIZE){
//             // center += (hash33(center*scale)*2.-1.) * node.radius;
//             // add_leaf(center, node.radius * 2.0, centerval);
//             continue; // TODO: make this a gas particle
//         }
//
//         // add chilren
//         for (uint i=0; i<8; ++i){
//             stack_push(children[i]);
//         }
//
//     }
//
// }
