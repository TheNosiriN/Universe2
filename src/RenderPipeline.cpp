#include "main.hpp"
using namespace Hexo::Graphics::Commands;


HXSIZE AddGraphicsPipeline(
    Application& app, RenderResourceRegistry& registry,  HXGraphicsPipelineStateConfig state, const std::string& file
){
    HXUINTP pip_bin;
    HX_GRAPHICS_ERROR_ASSERT_NO_RETURN(
        LoadFile(pip_bin, file), "Failed to load graphics pipeline"
    );

    HXGraphicsPipelineStateConfig nstate{};
    nstate.RasterizerConfig = state.RasterizerConfig;
    memcpy(nstate.BlendConfigs, state.BlendConfigs, sizeof(state.BlendConfigs));
    nstate.BlendCount = state.BlendCount;
    memcpy(nstate.RenderTargetFormats, state.RenderTargetFormats, sizeof(state.RenderTargetFormats));
    nstate.DepthStencilFormat = state.DepthStencilFormat;
    nstate.RenderTargetCount = state.RenderTargetCount;

    HXSIZE pip_id = registry.graphicsPipelines.emplace_back();
    HXGraphicsPipeline& pipeline = registry.graphicsPipelines[pip_id].pipeline;
    HXShaderUniformMap& uniforms = registry.graphicsPipelines[pip_id].uniforms;

    HXGraphicsPipelineConfig nconfig{};
    ShaderC::LoadShaderPipeline(pip_bin, &pipeline, &nconfig, &uniforms);
    app.hxg.CreateGraphicsPipeline(pipeline, nconfig, nstate);

    std::free(pip_bin);
    return HX_NULL_ID_MAKE_ID(pip_id);
}

HXSIZE AddComputePipeline(
    Application& app, RenderResourceRegistry& registry, const std::string& file
){
    HXUINTP pip_bin;
    HX_GRAPHICS_ERROR_ASSERT_NO_RETURN(
        LoadFile(pip_bin, file), "Failed to load compute pipeline"
    );

    HXSIZE pip_id = registry.computePipelines.emplace_back();
    HXComputePipeline& pipeline = registry.computePipelines[pip_id].pipeline;
    HXShaderUniformMap& uniforms = registry.computePipelines[pip_id].uniforms;

    HXComputePipelineConfig config{};
    ShaderC::LoadShaderPipeline(pip_bin, &pipeline, &config, &uniforms);
    app.hxg.CreateComputePipeline(pipeline, config);

    std::free(pip_bin);
    return HX_NULL_ID_MAKE_ID(pip_id);
}


void ConfigureDefaultUniformsStage(Application& app, RenderStage<0,0,0>* stage){
    if (!stage->InputTexCount)return;
    stage->uniforms[stage->UniformsCount] = HXShaderInput{
        app.hxg.GetUniform(app.renderer.registry.graphicsPipelines[HX_NULL_ID_GET_ID(stage->pipeline)].uniforms, "InputTexture"),
        NULL, (uint)stage->InputTexCount, &app.renderer.linearSampler
    };
}



HXStorageResourceConfig GetDefaultRenderTargetConfig(const uvec2& size){
    HXStorageResourceConfig config{};
    config.Type = HX_GRAPHICS_TEXTURE_RENDERTARGET;
    config.Width = size.x;
    config.Height = size.y;
    config.Depth = 1;
    config.MipCount = 1;
    config.Usage = HX_GRAPHICS_USAGE_GPU_R | HX_GRAPHICS_USAGE_GPU_W;
    config.Format = HX_R8_G8_B8_A8_UNORM;
    config.Data = NULL;
    return config;
}

HXSIZE AddRenderTarget(
    RenderResourceRegistry& registry, HXStorageResourceConfig& config, const uvec2& size, bool dependent, bool hdr=false
){
    HXSIZE texid = registry.renderTargets.emplace_back();
    registry.renderTargets[texid].size = size;
    registry.renderTargets[texid].hdr = hdr;
    config = GetDefaultRenderTargetConfig(size);
    config.Output = &registry.renderTargets[texid].tex;
    config.Format = hdr ? HX_R16_G16_B16_A16_FLOAT : config.Format;
    registry.renderTargets[texid].dependent = dependent;
    return HX_NULL_ID_MAKE_ID(texid);
}

HXSIZE AddDepthStencil(
    RenderResourceRegistry& registry, HXStorageResourceConfig& config, const uvec2& size, bool dependent
){
    HXSIZE texid = registry.depthStencils.emplace_back();
    registry.depthStencils[texid].size = size;
    config = GetDefaultRenderTargetConfig(size);
    config.Type = HX_GRAPHICS_TEXTURE_DEPTHSTENCIL;
    config.Format = HX_D32_FLOAT;
    config.Output = &registry.depthStencils[texid].tex;
    registry.depthStencils[texid].dependent = dependent;
    return HX_NULL_ID_MAKE_ID(texid);
}



void InitFrameBufferForStage(Application& app, RenderPipeline* pip, RenderStage<0,0,0>* stage){
    HXFrameBufferConfig rpconfig{};
    rpconfig.Flags = stage->renderPassFlags;
    rpconfig.DepthStencil = &(pip->registry.depthStencils[HX_NULL_ID_GET_ID(stage->depthStencil)].view);
    rpconfig.ClearFlags = stage->clearFlags;
    rpconfig.ClearColor = stage->clearColor;
    rpconfig.ClearDepth = 0.0f; // for logarithmic depth buffer

    rpconfig.RenderTargetCount = Mathgl::min(
        HX_GRAPHICS_DEF_MAX_RENDER_TARGETS_BOUND, stage->OutputTexCount
    );
    for(int i=0; i<rpconfig.RenderTargetCount; i++){
        rpconfig.RenderTargets[i] = &(
            pip->registry.renderTargets[HX_NULL_ID_GET_ID(stage->outputTextures[i])].view
        );
    }

    app.hxg.CreateFrameBuffer(rpconfig, stage->framebuffer);
}





RenderPipeline::RenderPipeline(){
    postStage.clearColor = vec4(0,0,0,0);
    postStage.renderPassFlags = HX_GRAPHICS_RENDERPASS_COLOUR;
    postStage.clearFlags = 0;
}



void RenderPipeline::Init(Application& app){
    // create pipelines
    HXGraphicsPipelineStateConfig state{};
    state.RasterizerConfig.CounterClockwise = false;
    state.RasterizerConfig.CullOp = HX_GRAPHICS_CULL_OP_NONE;
    state.RasterizerConfig.Mode = HX_GRAPHICS_DRAW_TRIANGLE;
    state.RenderTargetFormats[0] = HX_R8_G8_B8_A8_UNORM;
    state.RenderTargetCount = 1;
    state.DepthStencilFormat = HX_INPUT_FORMAT_NONE;
    presentPipeline = AddGraphicsPipeline(app, registry, state, "window_raster_shd.hxasset");
    HXSIZE fontRendererPipeline = AddGraphicsPipeline(app, registry, state, "font_renderer_raster_shd.hxasset");
    state.RenderTargetFormats[0] = HX_R16_G16_B16_A16_FLOAT;
    state.RenderTargetFormats[1] = HX_R8_G8_B8_A8_UNORM;
    state.RenderTargetCount = 2;
    HXSIZE postPipeline = AddGraphicsPipeline(app, registry, state, "post_process_raster_shd.hxasset");
    state.RenderTargetFormats[0] = HX_R16_G16_B16_A16_FLOAT;
    state.RenderTargetCount = 1;
    state.RasterizerConfig.CullOp = HX_GRAPHICS_CULL_OP_BACK;
    state.RasterizerConfig.DepthOp = HX_GRAPHICS_COMP_OP_GREATER_EQUAL;
    // HXSIZE gbufferSolidsPipeline = AddGraphicsPipeline(app, registry, state, "gbuffer_solids_raster_shd.hxasset");
    HXSIZE pointLightSpritePipeline = AddGraphicsPipeline(app, registry, state, "point_light_sprite_raster_shd.hxasset");
    state.RasterizerConfig.CullOp = HX_GRAPHICS_CULL_OP_FRONT;
    state.BlendConfigs[0].SrcFactor = HX_GRAPHICS_BLEND_ONE;
    state.BlendConfigs[0].DstFactor = HX_GRAPHICS_BLEND_ONE_MINUS_SRC_ALPHA;
    state.BlendConfigs[0].SrcAlphaFactor = HX_GRAPHICS_BLEND_ONE;
    state.BlendConfigs[0].DstAlphaFactor = HX_GRAPHICS_BLEND_ONE_MINUS_SRC_ALPHA;
    state.BlendCount = 1;
    state.RasterizerConfig.Mode = HX_GRAPHICS_DRAW_TRIANGLE_STRIP;
    HXSIZE universeActivePipeline = AddGraphicsPipeline(app, registry, state, "torus_universe_active_raster_shd.hxasset");
    state.RasterizerConfig.Mode = HX_GRAPHICS_DRAW_TRIANGLE;
    state.RasterizerConfig.CullOp = HX_GRAPHICS_CULL_OP_BACK;
    HXSIZE cosmicWebMatterPipeline = AddGraphicsPipeline(app, registry, state, "cosmic_web_matter_raster_shd.hxasset");

    HXSIZE cosmicWebDensityGen = AddComputePipeline(app, registry, "cosmicweb_density_gen_compute_shd.hxasset");
    HXSIZE cosmicWebDensityDraw = AddComputePipeline(app, registry, "cosmicweb_density_compute_shd.hxasset");

    // create contexts
    app.hxg.CreateGPUContext(globalContext, HX_GRAPHICS_CMDBUFFER_GRAPHICS);
    app.hxg.CreateGPUContext(computeContext, HX_GRAPHICS_CMDBUFFER_GRAPHICS);//HX_GRAPHICS_CMDBUFFER_COMPUTE);
    app.hxg.CreateGPUContext(copyContext, HX_GRAPHICS_CMDBUFFER_COPY);

    // create command buffers
    HXCommandBufferConfig cmd_buff_config{};
    cmd_buff_config.Type = HX_GRAPHICS_CMDBUFFER_GRAPHICS;
    cmd_buff_config.Usage = HX_GRAPHICS_CMDBUFFER_ONCE;
    app.hxg.CreateCommandBuffer(globalCommandBuffer, cmd_buff_config);
    cmd_buff_config.Type = HX_GRAPHICS_CMDBUFFER_COPY;
    app.hxg.CreateCommandBuffer(copyCommandBuffer, cmd_buff_config);
    cmd_buff_config.Type = HX_GRAPHICS_CMDBUFFER_GRAPHICS;//HX_GRAPHICS_CMDBUFFER_COMPUTE;
    app.hxg.CreateCommandBuffer(computeCommandBuffer, cmd_buff_config);

    // create storage buffers
    HXStorageResourceConfig world_buff_config{};
    world_buff_config.Data = NULL;
    world_buff_config.Usage = HX_GRAPHICS_USAGE_GPU_R | HX_GRAPHICS_USAGE_CPU_R | HX_GRAPHICS_USAGE_CPU_W;
    world_buff_config.Type = HX_GRAPHICS_UNIFORMBUFFER;
    world_buff_config.Width = sizeof(MultiverseWorldConstants);
    world_buff_config.Output = &worldBuffer;

    HXStorageResourceConfig cam_buff_config = world_buff_config;
    cam_buff_config.Width = sizeof(CameraConstants);
    cam_buff_config.Output = &cameraBuffer;

    app.hxg.CreateStorageResources(HXCommandExecutionState{}, world_buff_config, cam_buff_config);

    // create default samplers
    HXSamplerConfig linearSampConfig{};
    linearSampConfig.Filter = HX_GRAPHICS_SAMPLE_LINEAR;
    linearSampConfig.Output = &linearSampler;
    HXSamplerConfig nearestSamplerConfig{};
    nearestSamplerConfig.Filter = HX_GRAPHICS_SAMPLE_NEAREST;
    nearestSamplerConfig.Output = &nearestSampler;
    HXSamplerConfig mipmapSamplerConfig{};
    mipmapSamplerConfig.Filter = HX_GRAPHICS_SAMPLE_MIPMAP;
    mipmapSamplerConfig.Output = &mipmapSampler;
    app.hxg.CreateSamplers(linearSampConfig, nearestSamplerConfig, mipmapSamplerConfig);

    // create rendertarget textures
    HXSIZE gbufferColorRT, gbufferDepth, postProcessRT;
    HXSIZE transparentObjectsColorRT, fontColorUiRT;
    const HXSIZE texCount = 5;
    HXStorageResourceConfig configs[texCount];
    registry.renderTargets.reserve(texCount);
    registry.depthStencils.reserve(texCount);
    gbufferColorRT = AddRenderTarget(registry, configs[0], uvec2(2,2), true, true);
    gbufferDepth = AddDepthStencil(registry, configs[1], uvec2(2,2), true);
    postProcessRT = AddRenderTarget(registry, configs[2], uvec2(2,2), true, true);
    transparentObjectsColorRT = AddRenderTarget(registry, configs[3], uvec2(2,2), true);
    fontColorUiRT = AddRenderTarget(registry, configs[4], uvec2(2,2), true);

    HXCommandExecutionState copy_state{};
    copy_state.CommandBuffer = NULL;
    copy_state.Context = NULL;
    app.hxg.CreateStorageResourcesFromArray(copy_state, configs, sizeof(configs)/sizeof(HXStorageResourceConfig));

    app.hxg.CreateRenderTargetView(
        registry.renderTargets[HX_NULL_ID_GET_ID(gbufferColorRT)].view, globalContext,
        registry.renderTargets[HX_NULL_ID_GET_ID(gbufferColorRT)].tex, 0
    );
    app.hxg.CreateDepthStencilView(
        registry.depthStencils[HX_NULL_ID_GET_ID(gbufferDepth)].view, globalContext,
        registry.depthStencils[HX_NULL_ID_GET_ID(gbufferDepth)].tex, 0
    );
    app.hxg.CreateRenderTargetView(
        registry.renderTargets[HX_NULL_ID_GET_ID(postProcessRT)].view, globalContext,
        registry.renderTargets[HX_NULL_ID_GET_ID(postProcessRT)].tex, 0
    );
    app.hxg.CreateRenderTargetView(
        registry.renderTargets[HX_NULL_ID_GET_ID(transparentObjectsColorRT)].view, globalContext,
        registry.renderTargets[HX_NULL_ID_GET_ID(transparentObjectsColorRT)].tex, 0
    );
    app.hxg.CreateRenderTargetView(
        registry.renderTargets[HX_NULL_ID_GET_ID(fontColorUiRT)].view, globalContext,
        registry.renderTargets[HX_NULL_ID_GET_ID(fontColorUiRT)].tex, 0
    );

    // load useful textures
    auto whitenoise_img = LoadTexture("textures/whitenoise_rgba_2D.png", 256, 256, 4);
	auto bluenoise_img = LoadTexture("textures/bluenoise_rgba_2D.png", 512, 512, 4);

    HXStorageResourceConfig whitenoise_tex_config{};
    whitenoise_tex_config.Data = whitenoise_img;
    whitenoise_tex_config.Usage = HX_GRAPHICS_USAGE_GPU_R;
    whitenoise_tex_config.Type = HX_GRAPHICS_TEXTURE_2D;
    whitenoise_tex_config.Width = 256;
    whitenoise_tex_config.Height = 256;
    whitenoise_tex_config.Format = HX_R8_G8_B8_A8_UNORM;
    whitenoise_tex_config.Output = &whiteNoiseTex;

    HXStorageResourceConfig bluenoise_tex_config = whitenoise_tex_config;
    bluenoise_tex_config.Data = bluenoise_img;
    bluenoise_tex_config.Width = 512;
    bluenoise_tex_config.Height = 512;
    bluenoise_tex_config.Output = &blueNoiseTex;

    HXCommandExecutionState noisetex_config_state{};
    noisetex_config_state.Context = &copyContext;
    noisetex_config_state.CommandBuffer = &copyCommandBuffer;
    noisetex_config_state.StartCommandBuffer = true;
    noisetex_config_state.EndCommandBuffer = true;

    app.hxg.CreateStorageResources(noisetex_config_state, whitenoise_tex_config, bluenoise_tex_config);
    app.hxg.ExecuteCommandBuffers(copyContext, NULL, copyCommandBuffer);


    // configure stages
    postStage.pipeline = postPipeline;
    postStage.inputTextures[0] = gbufferColorRT;
    postStage.inputTextures[1] = transparentObjectsColorRT;
    postStage.inputTextures[2] = fontColorUiRT;
    postStage.type = HX_GRAPHICS_CMDBUFFER_GRAPHICS;

    // cleanup
    app.hxg.WaitForExecution(globalContext);
    app.hxg.WaitForExecution(copyContext);
    app.hxg.CleanUpResourceCopyStates(copy_state);
    app.hxg.CleanUpResourceCopyStates(noisetex_config_state);
    app.hxg.MapStorageBuffer(worldBuffer, 0, 0, (void**)(&worldBufferPtr));
    app.hxg.MapStorageBuffer(cameraBuffer, 0, 0, (void**)(&cameraBufferPtr));

    std::free(whitenoise_img);
    std::free(bluenoise_img);


    // store useful IDs
    app.StrMapStore("postPipeline", postPipeline);
    // app.StrMapStore("gbufferSolidsPipeline", gbufferSolidsPipeline);
    app.StrMapStore("fontRendererPipeline", fontRendererPipeline);
    app.StrMapStore("pointLightSpritePipeline", pointLightSpritePipeline);
    app.StrMapStore("universeActivePipeline", universeActivePipeline);
    app.StrMapStore("cosmicWebDensityGen", cosmicWebDensityGen);
    app.StrMapStore("cosmicWebDensityDraw", cosmicWebDensityDraw);
    app.StrMapStore("cosmicWebMatterPipeline", cosmicWebMatterPipeline);

    app.StrMapStore("postProcessRT", postProcessRT);
    app.StrMapStore("gbufferColorRT", gbufferColorRT);
    app.StrMapStore("gbufferDepth", gbufferDepth);
    app.StrMapStore("transparentObjectsColorRT", transparentObjectsColorRT);
    app.StrMapStore("fontColorUiRT", fontColorUiRT);


    gpuCopyQueue.Init(app);
}



void RenderPipeline::PreUpdate(Application& app){

}



void RenderPipeline::PostUpdate(Application& app){
    gpuCopyQueue.Update(app);
}



RenderChainID RenderPipeline::CreateRenderChain(Application& app, CommandBufferType type){
    RenderChain chain{};
    HXCommandBufferConfig cmd_buff_config{};
    cmd_buff_config.Type = HX_GRAPHICS_CMDBUFFER_GRAPHICS;//type;
    cmd_buff_config.Usage = HX_GRAPHICS_CMDBUFFER_ONCE;
    app.hxg.CreateCommandBuffer(chain.commandBuffer, cmd_buff_config);
    return HX_NULL_ID_MAKE_ID(chains.push_back(chain));
}



void RenderPipeline::AddRenderStage(Application& app, RenderChainID chainid, HXUINTP stage_ptr, RenderStageFunc func, HXUINTP data){
    RenderChain& chain = chains[HX_NULL_ID_GET_ID(chainid)];

    if (!chain.array.empty()){
        RenderStage<0,0,0>* lastStage = chain.array[chain.array.last_index()];
        if (lastStage->noFrameBuffer){ InitFrameBufferForStage(app, this, lastStage); }
    }

    RenderStage<0,0,0>* stage = stage_ptr;
    ConfigureDefaultUniformsStage(app, stage);
    stage->renderFunc = func;
    stage->active = true;
    stage->userdata = data;
    if (stage->type != HX_GRAPHICS_CMDBUFFER_GRAPHICS){ stage->noFrameBuffer = true; }
    chain.array.push_back(stage);
}



void RenderPipeline::InitCamera(Application& app, Camera& camera){
    HXSIZE ids[Camera::OutputStreamTexCount];
    HXStorageResourceConfig configs[Camera::OutputStreamTexCount];

    for (int i=0; i<Camera::OutputStreamTexCount; ++i){
        ids[i] = AddRenderTarget(registry, configs[i], uvec2(2,2), true, true);
        camera.outputStream.objects[i].tex = ids[i];
    }

    HXCommandExecutionState copy_state{};
    copy_state.CommandBuffer = NULL;
    copy_state.Context = NULL;
    copy_state.StartCommandBuffer = true;
    copy_state.EndCommandBuffer = true;
    app.hxg.CreateStorageResourcesFromArray(copy_state, configs, Camera::OutputStreamTexCount);

    for (int i=0; i<Camera::OutputStreamTexCount; ++i){
        app.hxg.CreateRenderTargetView(
            registry.renderTargets[HX_NULL_ID_GET_ID(ids[i])].view, globalContext,
            registry.renderTargets[HX_NULL_ID_GET_ID(ids[i])].tex, 0
        );

        HXFrameBufferConfig rpconfig{};
        rpconfig.Flags = HX_GRAPHICS_RENDERPASS_COLOUR;
        rpconfig.ClearFlags = 0;
        rpconfig.ClearColor = vec4(0);
        rpconfig.RenderTargetCount = 1;
        rpconfig.RenderTargets[0] = &(registry.renderTargets[HX_NULL_ID_GET_ID(ids[i])].view);
        app.hxg.CreateFrameBuffer(rpconfig, camera.outputStream.objects[i].framebuffer);
    }

    app.hxg.WaitForExecution(globalContext);
}



void RenderPipeline::StartStageRender(Application& app, RenderStage<0,0,0>* stage, HXCommandExecutionState& cmds_state){
    for (int i=0; i<stage->InputTexCount; ++i){
        HXChangeResourceState barr_cmd{};
        barr_cmd.state = HX_GRAPHICS_RESOURCE_STATE_SHADER_RESOURCE;
        barr_cmd.resource = &(registry.renderTargets[HX_NULL_ID_GET_ID(stage->inputTextures[i])].tex);
        app.hxg.InsertCommands(cmds_state, barr_cmd);
    }

    for (int i=0; i<stage->OutputTexCount; ++i){
        HXChangeResourceState barr_cmd{};
        barr_cmd.state = HX_GRAPHICS_RESOURCE_STATE_RENDER_TARGET;
        barr_cmd.resource = &(registry.renderTargets[HX_NULL_ID_GET_ID(stage->outputTextures[i])].tex);
        app.hxg.InsertCommands(cmds_state, barr_cmd);
    }

    for (int i=0; i<stage->InputTexCount; ++i){
        stage->internal_inputTextures[i] = registry.renderTargets[HX_NULL_ID_GET_ID(stage->inputTextures[i])].tex;
    }

    HXShaderInputsDesc uniforms{};
    uniforms.Inputs = stage->uniforms;

    if (stage->InputTexCount){
        stage->uniforms[stage->UniformsCount].Data = (stage->internal_inputTextures)+0;
        uniforms.Count = stage->UniformsCount + 1;
    }else{
        uniforms.Count = stage->UniformsCount;
    }

    if (stage->type == HX_GRAPHICS_CMDBUFFER_GRAPHICS){
        HXSetGraphicsPipeline setpip_cmd{};
        setpip_cmd.pipeline = &(registry.graphicsPipelines[HX_NULL_ID_GET_ID(stage->pipeline)].pipeline);
        setpip_cmd.uniforms = uniforms;
        app.hxg.InsertCommands(cmds_state, setpip_cmd);

        HXStartRenderPass rp_cmd{};
        rp_cmd.framebuffer = &stage->framebuffer;
        rp_cmd.viewport = vec4(0, 0, registry.renderTargets[HX_NULL_ID_GET_ID(stage->outputTextures[0])].size);
        app.hxg.InsertCommands(cmds_state, rp_cmd);
    }else{
        HXSetComputePipeline setpip_cmd{};
        setpip_cmd.pipeline = &(registry.computePipelines[HX_NULL_ID_GET_ID(stage->pipeline)].pipeline);
        setpip_cmd.uniforms = uniforms;
        app.hxg.InsertCommands(cmds_state, setpip_cmd);
    }
}

void RenderPipeline::EndStageRender(Application& app, RenderStage<0,0,0>* stage, HXCommandExecutionState& cmds_state){
    if (stage->type == HX_GRAPHICS_CMDBUFFER_GRAPHICS){
        HXEndRenderPass end_rp_cmd{};
        app.hxg.InsertCommands(cmds_state, end_rp_cmd);
    }
}



void RenderPipeline::Render(Application& app, HXGPUContext& context, RenderChainID chainid){
    RenderChain& chain = chains[HX_NULL_ID_GET_ID(chainid)];

    HXCommandExecutionState cmds_state{};
    cmds_state.CommandBuffer = &chain.commandBuffer;
    cmds_state.Context = &context;

    int totalUniformCount = 0;
    for (int i=0; i<chain.array.size(); ++i){
        RenderStage<0,0,0>* stage = chain.array[i];
        if (!stage->active)return;
        totalUniformCount += stage->UniformsCount;
    }

    app.hxg.FenceWaitGPU(context, chain.fence);
    app.hxg.ResetGPUContext(context);
    app.hxg.StartRecording(cmds_state, totalUniformCount);

    for (int i=0; i<chain.array.size(); ++i){
        RenderStage<0,0,0>* stage = chain.array[i];
        if (!stage->renderFunc || !stage->active)continue;
        StartStageRender(app, stage, cmds_state);
        stage->renderFunc(&app, this, cmds_state, &stage, NULL, stage->userdata);
        EndStageRender(app, stage, cmds_state);
    }

    app.hxg.StopRecording(cmds_state);
    app.hxg.ExecuteCommandBuffers(context, &chain.fence, chain.commandBuffer);
}


void RenderPipeline::Render(
    Application& app, HXGPUContext& context, RenderChainID chainid, Camera* camera, BufferedTextureOutput& textureStream
){
    RenderChain& chain = chains[HX_NULL_ID_GET_ID(chainid)];
    RenderStage<0,0,0>* lastStage = chain.array[chain.array.last_index()];
    lastStage->outputTextures[0] = textureStream.tex;
    lastStage->framebuffer = textureStream.framebuffer;

    // copy camera to buffer
    if (camera){
        CameraConstants gpu_cam{};
        gpu_cam.projection = camera->projection;
        gpu_cam.view = camera->view;
        gpu_cam.viewProjInv = inverse(gpu_cam.projection * gpu_cam.view);
        gpu_cam.frameIndex = app.frame_index;
        gpu_cam.resolution = camera->resolution;
        memcpy(cameraBufferPtr, &gpu_cam, sizeof(CameraConstants));
    }

    HXCommandExecutionState cmds_state{};
    cmds_state.CommandBuffer = &chain.commandBuffer;
    cmds_state.Context = &context;

    int totalUniformCount = 0;
    for (int i=0; i<chain.array.size(); ++i){
        RenderStage<0,0,0>* stage = chain.array[i];
        if (!stage->active)return;
        totalUniformCount += stage->UniformsCount;
        totalUniformCount += stage->InputTexCount;
    }

    app.hxg.FenceWaitGPU(context, textureStream.fence);
    app.hxg.ResetGPUContext(context);
    app.hxg.StartRecording(cmds_state, totalUniformCount);

    for (int i=0; i<chain.array.size(); ++i){
        RenderStage<0,0,0>* stage = chain.array[i];
        if (!stage->renderFunc || !stage->active)continue;
        StartStageRender(app, stage, cmds_state);
        stage->renderFunc(&app, this, cmds_state, &stage, camera, stage->userdata);
        EndStageRender(app, stage, cmds_state);
    }

    app.hxg.StopRecording(cmds_state);
    app.hxg.ExecuteCommandBuffers(context, &textureStream.fence, chain.commandBuffer);
}



void RenderPipeline::Resize(Application& app, const uvec2& size){
    HXCommandExecutionState copy_state{};
    copy_state.CommandBuffer = NULL;
    copy_state.Context = NULL;

    // reset chains
    for (int i=0; i<chains.size(); ++i){
        app.hxg.ResetCommandBuffer(chains[i].commandBuffer);
    }

    // re-create resources
    for (int i=0; i<registry.renderTargets.size(); ++i){
        auto& rt = registry.renderTargets[i];
        if (!rt.dependent)continue;
        app.hxg.DestroyTextures(rt.tex);

        HXStorageResourceConfig config = GetDefaultRenderTargetConfig(size);
        config.Output = &rt.tex;
        config.Format = rt.hdr ? HX_R16_G16_B16_A16_FLOAT : config.Format;
        app.hxg.CreateStorageResources(copy_state, config);
        rt.size = size;
    }

    for (int i=0; i<registry.depthStencils.size(); ++i){
        auto& rt = registry.depthStencils[i];
        if (!rt.dependent)continue;
        app.hxg.DestroyTextures(rt.tex);

        HXStorageResourceConfig config = GetDefaultRenderTargetConfig(size);
        config.Type = HX_GRAPHICS_TEXTURE_DEPTHSTENCIL;
        config.Format = HX_D32_FLOAT;
        config.Output = &rt.tex;
        app.hxg.CreateStorageResources(copy_state, config);
        rt.size = size;
    }

    app.hxg.WaitForExecution(globalContext);

    // reset everything
    app.hxg.ResetCommandBuffer(globalCommandBuffer);
    app.hxg.ResetGPUContext(globalContext);

    // re-create views
    for (int i=0; i<registry.renderTargets.size(); ++i){
        auto& rt = registry.renderTargets[i];
        if (!rt.dependent)continue;
        app.hxg.CreateRenderTargetView(rt.view, globalContext, rt.tex, 0);
    }

    for (int i=0; i<registry.depthStencils.size(); ++i){
        auto& rt = registry.depthStencils[i];
        if (!rt.dependent)continue;
        app.hxg.CreateDepthStencilView(rt.view, globalContext, rt.tex, 0);
    }

    // recreate framebuffers
    for (int i=0; i<chains.size(); ++i){
        for (int j=0; j<chains[i].array.size(); ++j){
            RenderStage<0,0,0>* stage = chains[i].array[j];
            if (stage->noFrameBuffer)continue;
            app.hxg.DestroyFrameBuffer(stage->framebuffer);
            InitFrameBufferForStage(app, this, stage);
        }
    }
}


void RenderPipeline::Destroy(Application& app){

    app.hxg.UnmapStorageBuffer(cameraBuffer, 0, 0);

}








void CopyQueueManagerGPU::Init(Application& app){
    queue_size = 32 * 1024 * 1024; // 32MB

    HXStorageResourceConfig upl_buff_config{};
    upl_buff_config.Data = NULL;
    upl_buff_config.Usage = HX_GRAPHICS_USAGE_GPU_R | HX_GRAPHICS_USAGE_CPU_W;
    upl_buff_config.Type = HX_GRAPHICS_STORAGEBUFFER;
    upl_buff_config.Width = queue_size;
    upl_buff_config.Output = &uploadBuffer;

    HXStorageResourceConfig read_buff_config{};
    read_buff_config.Data = NULL;
    read_buff_config.Usage = HX_GRAPHICS_USAGE_GPU_W | HX_GRAPHICS_USAGE_CPU_R;
    read_buff_config.Type = HX_GRAPHICS_STORAGEBUFFER;
    read_buff_config.Width = queue_size;
    read_buff_config.Output = &readbackBuffer;

    app.hxg.CreateStorageResources(HXCommandExecutionState{}, upl_buff_config, read_buff_config);
    app.hxg.MapStorageBuffer(uploadBuffer, 0, 0, (void**)(&uploadBufferPtr));
    app.hxg.MapStorageBuffer(readbackBuffer, 0, 0, (void**)(&readbackBufferPtr));
}


void CopyQueueManagerGPU::Start(Application& app){
    HXCommandExecutionState cmd_state{};
    cmd_state.CommandBuffer = &app.renderer.copyCommandBuffer;
    cmd_state.Context = &app.renderer.copyContext;

    if (app.frame_index % 100){
        app.hxg.FenceWaitCPU(fence);
        app.hxg.ResetGPUContext(app.renderer.copyContext);
    }

    app.hxg.StartRecordingCopy(cmd_state);
    started = true;
}


void CopyQueueManagerGPU::Update(Application& app){
    if (!started)return;

    HXCommandExecutionState cmd_state{};
    cmd_state.CommandBuffer = &app.renderer.copyCommandBuffer;
    cmd_state.Context = &app.renderer.copyContext;
    app.hxg.StopRecording(cmd_state);
    started = false;
    uploadSize = 0;
    readbackSize = 0;

    app.hxg.FenceWaitGPU(app.renderer.copyContext, fence);
    app.hxg.ExecuteCommandBuffers(app.renderer.copyContext, &fence, app.renderer.copyCommandBuffer);
}


HXUINTP CopyQueueManagerGPU::AddReadRequest(
    Application& app, HXStorageBuffer* buffer, const HXSIZE& offset, const HXSIZE& size
){
    if (readbackSize+size > queue_size)return NULL;
    HXSIZE outoffset = readbackSize;
    readbackSize += size;
    if (!started){ this->Start(app); }

    HXChangeResourceState barr_cmd_bef{};
    barr_cmd_bef.resource = buffer;
    barr_cmd_bef.state = HX_GRAPHICS_RESOURCE_STATE_COPY_SRC;

    HXCopyStorageBuffer copy_cmd{};
    copy_cmd.src = buffer;
    copy_cmd.dst = &readbackBuffer;
    copy_cmd.srcOffset = offset;
    copy_cmd.dstOffset = outoffset;
    copy_cmd.size = size;

    HXChangeResourceState barr_cmd_aft = barr_cmd_bef;
    barr_cmd_aft.state = HX_GRAPHICS_RESOURCE_STATE_DEFAULT;

    HXCommandExecutionState cmd_state{};
    cmd_state.CommandBuffer = &app.renderer.copyCommandBuffer;
    cmd_state.Context = &app.renderer.copyContext;
    app.hxg.InsertCommands(cmd_state, barr_cmd_bef, copy_cmd, barr_cmd_aft);

    return readbackBufferPtr + outoffset;
}


HXUINTP CopyQueueManagerGPU::AddWriteRequest(
    Application& app, HXStorageBuffer* buffer, const HXSIZE& offset, const HXSIZE& size
){
    if (uploadSize+size > queue_size)return NULL;
    HXSIZE outoffset = uploadSize;
    uploadSize += size;
    if (!started){ this->Start(app); }

    HXChangeResourceState barr_cmd_bef{};
    barr_cmd_bef.resource = buffer;
    barr_cmd_bef.state = HX_GRAPHICS_RESOURCE_STATE_COPY_DST;

    HXCopyStorageBuffer copy_cmd{};
    copy_cmd.src = &uploadBuffer;
    copy_cmd.dst = buffer;
    copy_cmd.srcOffset = outoffset;
    copy_cmd.dstOffset = offset;
    copy_cmd.size = size;

    HXChangeResourceState barr_cmd_aft = barr_cmd_bef;
    barr_cmd_aft.state = HX_GRAPHICS_RESOURCE_STATE_DEFAULT;

    HXCommandExecutionState cmd_state{};
    cmd_state.CommandBuffer = &app.renderer.copyCommandBuffer;
    cmd_state.Context = &app.renderer.copyContext;
    app.hxg.InsertCommands(cmd_state, barr_cmd_bef, copy_cmd, barr_cmd_aft);

    return uploadBufferPtr + outoffset;
}
