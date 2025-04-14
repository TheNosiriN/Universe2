#include "main.hpp"

static void _ParticleOctree_ComputeOctree(RSI_RENDERSTAGE_PARAMS){
    ParticleOctree* octree = userdata;
    Commands::HXDispatchComputeWork dispatchcmd{};
    dispatchcmd.x = size_t(ceil(octree->properties.gridSize.x / 8));
    dispatchcmd.y = size_t(ceil(octree->properties.gridSize.y / 8));
    dispatchcmd.z = size_t(ceil(octree->properties.gridSize.z / 8));
    app->hxg.InsertCommands(cmds_state, dispatchcmd);
    // octree->computeStage.active = false;
}



void ParticleOctree::Init(
    Application& app, SpaceEngine& engine, const RenderChainID& chain,
    const HXSIZE& pipeline, const uvec3& startGrid, const uint& maxDepth
){
    this->computeChain = chain;
    this->properties.gridSize = startGrid;
    this->properties.maxDepth = float(maxDepth);
    this->properties.minGridSize = (float)min(startGrid.x, min(startGrid.y, startGrid.z));

    vec3 nodecount = vec3(startGrid) * float(1 << maxDepth);//pow(2.0f, MaxDepth);
    uint maxNodes = uint(nodecount.x * nodecount.y * nodecount.z);

    // create storage buffers
    HXStorageResourceConfig leafs_buff_config{};
    leafs_buff_config.Data = NULL;
    leafs_buff_config.Usage = HX_GRAPHICS_USAGE_GPU_R | HX_GRAPHICS_USAGE_GPU_W;
    leafs_buff_config.Type = HX_GRAPHICS_STORAGEBUFFER;
    leafs_buff_config.Width = sizeof(ParticleOctreeNode) * maxNodes;
    leafs_buff_config.Output = &leafNodes;

    HXStorageResourceConfig atm_buff_config = leafs_buff_config;
    atm_buff_config.Type = HX_GRAPHICS_ATOMICCOUNTER_BUFFER;
    atm_buff_config.Width = sizeof(uint);
    atm_buff_config.Output = &atomicCounters;

    app.hxg.CreateStorageResources(HXCommandExecutionState{}, leafs_buff_config, atm_buff_config);

    // create stage
    computeStage.active = false;
    computeStage.type = HX_GRAPHICS_CMDBUFFER_COMPUTE;
    computeStage.pipeline = app.StrMapGet("cosmicWebOctreeGen");
    HXShaderUniformMap& uniforms = app.renderer.registry.computePipelines[HX_NULL_ID_GET_ID(computeStage.pipeline)].uniforms;
    computeStage.uniforms[0] = HXShaderInput{ app.hxg.GetUniform(uniforms, "LeafNodes"), &leafNodes };
    computeStage.uniforms[1] = HXShaderInput{ app.hxg.GetUniform(uniforms, "LeafCounter"), &atomicCounters };
    computeStage.uniforms[2] = HXShaderInput{ app.hxg.GetUniform(uniforms, "Props"), &properties };
    app.renderer.AddRenderStage(app, computeChain, &computeStage, &_ParticleOctree_ComputeOctree, this);

    // add nodes to rebuild
    RebuildGrid(app);
}


void ParticleOctree::Update(Application& app, SpaceEngine& engine){
    HXUINTP readptr = app.renderer.gpuCopyQueue.AddReadRequest(app, &atomicCounters, 0, sizeof(uint));
    if (readptr){ memcpy(&curParticleCount, readptr, sizeof(uint)); }

    if (computeStage.active){
        // zero the atomic counter
        // HXUINTP writeptr = app.renderer.gpuCopyQueue.AddWriteRequest(app, &atomicCounters, 0, sizeof(uint));
        // if (writeptr){ memset(writeptr, 0, sizeof(uint)); }
    }

    // std::cout << "x: " << properties.position.x << ", ";
    // std::cout << "y: " << properties.position.y << ", ";
    // std::cout << "z: " << properties.position.z << "\n";
}


void ParticleOctree::CleanUp(Application& app, SpaceEngine& engine){

}


void ParticleOctree::RebuildGrid(Application& app){
    computeStage.active = true;
}
