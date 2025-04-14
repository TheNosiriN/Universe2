#include "main.hpp"


void _StaticParticleBlock_RenderBlock(RSI_RENDERSTAGE_PARAMS){
    StaticParticlesBlock* block = userdata;

    Commands::HXVertexDraw vdraw{};
    vdraw.StartVertex = 0;
    vdraw.StartInstance = 0;
    vdraw.VertexCount = 3;
    vdraw.InstanceCount = (
        block->properties.gridSize * block->properties.gridSize * block->properties.gridSize * block->properties.levels
    );
    vdraw.VDesc = NULL;
    vdraw.BufferCount = 0;
    app->hxg.InsertCommands(cmds_state, vdraw);
}



void StaticParticlesBlock::Init(
    Application& app, SpaceEngine& engine, const RenderChainID& chain,
    const uint& gridSize, const uint& levels
){
    this->properties.gridSize = gridSize;
    this->properties.levels = min(levels, uint(MAX_STATIC_PARTICLES_BLOCK_DEPTH));

    // create storage buffers
    HXStorageResourceConfig relpos_buff_config{};
    relpos_buff_config.Data = NULL;
    relpos_buff_config.Usage = HX_GRAPHICS_USAGE_GPU_R | HX_GRAPHICS_USAGE_CPU_W;
    relpos_buff_config.Type = HX_GRAPHICS_STORAGEBUFFER;
    relpos_buff_config.Width = sizeof(vec4) * levels;
    relpos_buff_config.Output = &relposBuffer;
    app.hxg.CreateStorageResources(HXCommandExecutionState{}, relpos_buff_config);
    app.hxg.MapStorageBuffer(relposBuffer, 0, 0, (void**)(&relposBufferPtr));

    // create stage
    if (!shaderStage.pipeline)return;
    HXShaderUniformMap& uniforms = app.renderer.registry.graphicsPipelines[HX_NULL_ID_GET_ID(shaderStage.pipeline)].uniforms;
    shaderStage.uniforms[0] = HXShaderInput{ app.hxg.GetUniform(uniforms, "CameraData"), &app.renderer.cameraBuffer };
    shaderStage.uniforms[1] = HXShaderInput{ app.hxg.GetUniform(uniforms, "RelPosBuffer"), &relposBuffer };
    shaderStage.uniforms[2] = HXShaderInput{ app.hxg.GetUniform(uniforms, "Props"), &properties };
    app.renderer.AddRenderStage(app, chain, &shaderStage, &_StaticParticleBlock_RenderBlock, this);
}



InactiveSpaceObjectGPU StaticParticlesBlock::Update(Application& app, SpaceEngine& engine, const SpaceObject& parent){
    vec4 positions[MAX_STATIC_PARTICLES_BLOCK_DEPTH];

    spc_dvec3 accumPos, rel_position;
    SpaceObject lastobj = parent;
    InactiveSpaceObjectGPU rootgpu = engine.UpdateInactiveSpaceObject(&lastobj, NULL, accumPos, rel_position);
    positions[0] = vec4(rootgpu.position, 1);

    for (uint i=1u; i<this->properties.levels; ++i){
        uint level = i+1u;
        float levelscale = 1.0f/float(level);
        SpaceObject spcobj = engine.CreateSpaceObject<SpaceObject>(spc_dvec3(0), parent.radius*levelscale, 1.0f);
        InactiveSpaceObjectGPU gpu = engine.UpdateInactiveSpaceObject(&spcobj, &lastobj, accumPos, rel_position);
        positions[i] = vec4(gpu.position, levelscale);
        lastobj = spcobj;
    }

    hxmemcpy(relposBufferPtr, positions, sizeof(vec4)*this->properties.levels);
    return rootgpu;
}



void StaticParticlesBlock::CleanUp(Application& app, SpaceEngine& engine){

}
