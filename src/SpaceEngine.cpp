#include "main.hpp"
using namespace Hexo::Graphics::Commands;



HXStorageBuffer cube_vbuff{};
HXStorageBuffer cube_ibuff{};
HXVertexBufferViewPack<1> cube_vbuff_view;
HXIndexBufferView cube_ibuff_view;

HXStorageBuffer torus_vbuff{};
HXStorageBuffer torus_ibuff{};
HXVertexBufferViewPack<1> torus_vbuff_view;
HXIndexBufferView torus_ibuff_view;

HXStorageBuffer icosphere_vbuff{};
HXStorageBuffer icosphere_ibuff{};
HXVertexBufferViewPack<1> icosphere_vbuff_view;
HXIndexBufferView icosphere_ibuff_view;

const uint torus_slices = 80;
const uint torus_loops = 40;
const float torus_inner_rad = 0.490f;
const float torus_outer_rad = 0.510f;


static void CreateCube(Application& app){
    HXStorageResourceConfig vbuff_config{};
    vbuff_config.Data = cube_vertices+0;
    vbuff_config.Width = sizeof(cube_vertices);
    vbuff_config.Usage = HX_GRAPHICS_USAGE_GPU_R;
    vbuff_config.Type = HX_GRAPHICS_VERTEXBUFFER;
    vbuff_config.Output = &cube_vbuff;

    HXStorageResourceConfig ibuff_config{};
    ibuff_config.Data = cube_indices+0;
    ibuff_config.Width = sizeof(cube_indices);
    ibuff_config.Usage = HX_GRAPHICS_USAGE_GPU_R;
    ibuff_config.Type = HX_GRAPHICS_INDEXBUFFER;
    ibuff_config.Output = &cube_ibuff;

    HXCommandExecutionState copy_state{};
    copy_state.CommandBuffer = &app.renderer.copyCommandBuffer;
    copy_state.Context = &app.renderer.copyContext;
    copy_state.StartCommandBuffer = true;
    copy_state.EndCommandBuffer = true;

    app.hxg.CreateStorageResources(copy_state, vbuff_config, ibuff_config);

    app.hxg.ExecuteCommandBuffers(app.renderer.copyContext, NULL, app.renderer.copyCommandBuffer);
    app.hxg.WaitForExecution(app.renderer.copyContext);
    app.hxg.CleanUpResourceCopyStates(copy_state);

    app.hxg.CreateVertexBufferView(cube_vbuff_view,
        HXVBufferDesc{ &cube_vbuff, sizeof(float)*3, sizeof(cube_vertices)/(sizeof(float)*3) }
    );
    app.hxg.CreateIndexBufferView(cube_ibuff_view, cube_ibuff, sizeof(cube_indices)/sizeof(uint32_t), HX_R32_UINT);
}

static void CreateHornTorus(Application& app){
    uint vertex_total_size = (torus_slices+1) * (torus_loops+1);
    uint index_total_size = torus_slices * torus_loops * 6;

    vec3* vertices = (vec3*)malloc(vertex_total_size * sizeof(vec3));
    // vec2* uvs = (vec2*)malloc(vertex_total_size * sizeof(vec2));
    uint* indices = (uint*)malloc(index_total_size * sizeof(uint));
    ConstructHornTorusMeshVertices(torus_slices, torus_loops, torus_inner_rad, torus_outer_rad, vertices, indices, NULL);

    HXStorageResourceConfig vbuff_config{};
    vbuff_config.Data = vertices;
    vbuff_config.Width = vertex_total_size * sizeof(vec3);
    vbuff_config.Usage = HX_GRAPHICS_USAGE_GPU_R;
    vbuff_config.Type = HX_GRAPHICS_VERTEXBUFFER;
    vbuff_config.Output = &torus_vbuff;

    HXStorageResourceConfig ibuff_config{};
    ibuff_config.Data = indices;
    ibuff_config.Width = index_total_size * sizeof(uint);
    ibuff_config.Usage = HX_GRAPHICS_USAGE_GPU_R;
    ibuff_config.Type = HX_GRAPHICS_INDEXBUFFER;
    ibuff_config.Output = &torus_ibuff;

    HXCommandExecutionState copy_state{};
    copy_state.CommandBuffer = &app.renderer.copyCommandBuffer;
    copy_state.Context = &app.renderer.copyContext;
    copy_state.StartCommandBuffer = true;
    copy_state.EndCommandBuffer = true;

    app.hxg.CreateStorageResources(copy_state, vbuff_config, ibuff_config);
    app.hxg.ExecuteCommandBuffers(app.renderer.copyContext, NULL, app.renderer.copyCommandBuffer);
    app.hxg.WaitForExecution(app.renderer.copyContext);
    app.hxg.CleanUpResourceCopyStates(copy_state);
    app.hxg.CreateVertexBufferView(torus_vbuff_view,
        HXVBufferDesc{ &torus_vbuff, sizeof(vec3), vertex_total_size }
        // HXVBufferDesc{ &torus_ubuff, sizeof(vec2), (slices+1)*(loops+1) },
    );
    app.hxg.CreateIndexBufferView(torus_ibuff_view, torus_ibuff, index_total_size, HX_R32_UINT);

    std::free(vertices);
    std::free(indices);
}

static void CreateIcoSphere(Application& app){
    HXStorageResourceConfig vbuff_config{};
    vbuff_config.Data = icosphere_vertices+0;
    vbuff_config.Width = sizeof(icosphere_vertices);
    vbuff_config.Usage = HX_GRAPHICS_USAGE_GPU_R;
    vbuff_config.Type = HX_GRAPHICS_VERTEXBUFFER;
    vbuff_config.Output = &icosphere_vbuff;

    HXStorageResourceConfig ibuff_config{};
    ibuff_config.Data = icosphere_indices+0;
    ibuff_config.Width = sizeof(icosphere_indices);
    ibuff_config.Usage = HX_GRAPHICS_USAGE_GPU_R;
    ibuff_config.Type = HX_GRAPHICS_INDEXBUFFER;
    ibuff_config.Output = &icosphere_ibuff;

    HXCommandExecutionState copy_state{};
    copy_state.CommandBuffer = &app.renderer.copyCommandBuffer;
    copy_state.Context = &app.renderer.copyContext;
    copy_state.StartCommandBuffer = true;
    copy_state.EndCommandBuffer = true;

    app.hxg.CreateStorageResources(copy_state, vbuff_config, ibuff_config);

    app.hxg.ExecuteCommandBuffers(app.renderer.copyContext, NULL, app.renderer.copyCommandBuffer);
    app.hxg.WaitForExecution(app.renderer.copyContext);
    app.hxg.CleanUpResourceCopyStates(copy_state);

    app.hxg.CreateVertexBufferView(icosphere_vbuff_view,
        HXVBufferDesc{ &icosphere_vbuff, sizeof(float)*3, sizeof(icosphere_vertices)/(sizeof(float)*3) }
    );
    app.hxg.CreateIndexBufferView(icosphere_ibuff_view, icosphere_ibuff, sizeof(icosphere_indices)/sizeof(uint32_t), HX_R32_UINT);
}




void RenderPointLightSprites(RSI_RENDERSTAGE_PARAMS){
    HXVertexDraw vdraw{};
    vdraw.StartVertex = 0;
    vdraw.StartInstance = 0;
    vdraw.VertexCount = 6;
    vdraw.InstanceCount = app->space.pls_obj_range.count;
    vdraw.VDesc = NULL;
    vdraw.BufferCount = 0;
    if (!vdraw.InstanceCount)return;
    app->hxg.InsertCommands(cmds_state, vdraw);
}

void RenderFarUniverses(RSI_RENDERSTAGE_PARAMS){
    HXIndexDraw idraw{};
    idraw.StartIndex = 0;
    idraw.StartInstance = 0;
    idraw.IndexCount = UINT_MAX;
    idraw.InstanceCount = 1;//app->space.far_universes_obj_range.count;
    idraw.VDesc = &torus_vbuff_view;
    idraw.IDesc = &torus_ibuff_view;
    if (!idraw.InstanceCount)return;
    app->hxg.InsertCommands(cmds_state, idraw);
}

void RenderActiveUniverses(RSI_RENDERSTAGE_PARAMS){
    HXVertexDraw vdraw{};
    vdraw.StartVertex = 0;
    vdraw.StartInstance = 0;
    vdraw.VertexCount = 14;
    vdraw.InstanceCount = app->space.active_universes_obj_range.count;
    vdraw.VDesc = NULL;
    vdraw.BufferCount = 0;
    if (!vdraw.InstanceCount)return;
    app->hxg.InsertCommands(cmds_state, vdraw);

    // HXIndexDraw idraw{};
    // idraw.StartIndex = 0;
    // idraw.StartInstance = 0;
    // idraw.IndexCount = UINT_MAX;
    // idraw.InstanceCount = app->space.active_universes_obj_range.count;
    // idraw.VDesc = &icosphere_vbuff_view;
    // idraw.IDesc = &icosphere_ibuff_view;
    // idraw.Mode = HX_GRAPHICS_DRAW_TRIANGLE;
    // app->hxg.InsertCommands(cmds_state, idraw);
}

void RenderCosmicWebMatter(RSI_RENDERSTAGE_PARAMS){
    SpaceEngine* space = userdata;

    HXVertexDraw vdraw{};
    vdraw.StartVertex = 0;
    vdraw.StartInstance = 0;
    vdraw.VertexCount = 3;
    vdraw.InstanceCount = 32 * 32 * 32 * 8;
    vdraw.VDesc = NULL;
    vdraw.BufferCount = 0;
    app->hxg.InsertCommands(cmds_state, vdraw);

    // std::cout << "count: " << vdraw.InstanceCount << '\n';
}

void RenderGBufferSolids(RSI_RENDERSTAGE_PARAMS){
    HXIndexDraw idraw{};
    idraw.StartIndex = 0;
    idraw.StartInstance = 0;
    idraw.IndexCount = UINT_MAX;
    idraw.InstanceCount = 0;
    idraw.VDesc = &cube_vbuff_view;
    idraw.IDesc = &cube_ibuff_view;
    if (!idraw.InstanceCount)return;
    app->hxg.InsertCommands(cmds_state, idraw);
}



void SpaceEngine::SetupPipelines(Application& app){
    {
        pointLightSpriteStage.type = HX_GRAPHICS_CMDBUFFER_GRAPHICS;
        pointLightSpriteStage.clearColor = vec4(0,0,0,0);
        pointLightSpriteStage.renderPassFlags = HX_GRAPHICS_RENDERPASS_COLOUR;
        pointLightSpriteStage.clearFlags = pointLightSpriteStage.renderPassFlags;
        pointLightSpriteStage.outputTextures[0] = app.StrMapGet("gbufferColorRT");
        pointLightSpriteStage.pipeline = app.StrMapGet("pointLightSpritePipeline");
        HXShaderUniformMap& uniforms = app.renderer.registry.graphicsPipelines[HX_NULL_ID_GET_ID(pointLightSpriteStage.pipeline)].uniforms;
        pointLightSpriteStage.uniforms[0] = HXShaderInput{ app.hxg.GetUniform(uniforms, "CameraData"), &app.renderer.cameraBuffer };
        pointLightSpriteStage.uniforms[1] = HXShaderInput{ app.hxg.GetUniform(uniforms, "PointLights"), &pointLightSpritesObjBuffer };
    }
    {
        universeActiveStage.type = HX_GRAPHICS_CMDBUFFER_GRAPHICS;
        universeActiveStage.clearColor = vec4(0,0,0,0);
        universeActiveStage.renderPassFlags = HX_GRAPHICS_RENDERPASS_COLOUR;
        universeActiveStage.clearFlags = 0;
        universeActiveStage.outputTextures[0] = app.StrMapGet("gbufferColorRT");
        universeActiveStage.pipeline = app.StrMapGet("universeActivePipeline");
        HXShaderUniformMap& uniforms = app.renderer.registry.graphicsPipelines[HX_NULL_ID_GET_ID(universeActiveStage.pipeline)].uniforms;
        universeActiveStage.uniforms[0] = HXShaderInput{ app.hxg.GetUniform(uniforms, "WorldData"), &app.renderer.worldBuffer };
        universeActiveStage.uniforms[1] = HXShaderInput{ app.hxg.GetUniform(uniforms, "CameraData"), &app.renderer.cameraBuffer };
        universeActiveStage.uniforms[2] = HXShaderInput{ app.hxg.GetUniform(uniforms, "SpaceObjects"), &activeObjectsBuffer };
        universeActiveStage.uniforms[3] = HXShaderInput{ app.hxg.GetUniform(uniforms, "blueNoiseTex"), &app.renderer.blueNoiseTex };
    }
    {
        // cosmicWebMatterStage.type = HX_GRAPHICS_CMDBUFFER_GRAPHICS;
        // cosmicWebMatterStage.clearColor = vec4(0,0,0,0);
        // cosmicWebMatterStage.renderPassFlags = HX_GRAPHICS_RENDERPASS_COLOUR;
        // cosmicWebMatterStage.clearFlags = 0;
        // cosmicWebMatterStage.outputTextures[0] = app.StrMapGet("gbufferColorRT");
        // cosmicWebMatterStage.pipeline = app.StrMapGet("cosmicWebMatterPipeline");
        // cosmicWebMatterStage.depthStencil = NULL;
        // HXShaderUniformMap& uniforms = app.renderer.registry.graphicsPipelines[HX_NULL_ID_GET_ID(cosmicWebMatterStage.pipeline)].uniforms;
        // cosmicWebMatterStage.uniforms[0] = HXShaderInput{ app.hxg.GetUniform(uniforms, "CameraData"), &app.renderer.cameraBuffer };
        // cosmicWebMatterStage.uniforms[1] = HXShaderInput{ app.hxg.GetUniform(uniforms, "ParentProps"), NULL };
        // cosmicWebMatterStage.uniforms[2] = HXShaderInput{ app.hxg.GetUniform(uniforms, "ParticlesBuffer"), &universeOctree.leafNodes };
    }
    {
        // gbufferSolidsStage.type = HX_GRAPHICS_CMDBUFFER_GRAPHICS;
        // gbufferSolidsStage.clearColor = vec4(0,0,0,0);
        // gbufferSolidsStage.renderPassFlags = HX_GRAPHICS_RENDERPASS_COLOUR | HX_GRAPHICS_RENDERPASS_DEPTH;
        // gbufferSolidsStage.clearFlags = HX_GRAPHICS_RENDERPASS_DEPTH;//gbufferSolidsStage.renderPassFlags;
        // gbufferSolidsStage.outputTextures[0] = app.StrMapGet("gbufferColorRT");
        // gbufferSolidsStage.depthStencil = app.StrMapGet("gbufferDepth");
        // gbufferSolidsStage.pipeline = app.StrMapGet("gbufferSolidsPipeline");
        // HXShaderUniformMap& uniforms = app.renderer.registry.graphicsPipelines[HX_NULL_ID_GET_ID(gbufferSolidsStage.pipeline)].uniforms;
        // gbufferSolidsStage.uniforms[0] = HXShaderInput{ app.hxg.GetUniform(uniforms, "CameraData"), &app.renderer.cameraBuffer };
        // gbufferSolidsStage.uniforms[1] = HXShaderInput{ app.hxg.GetUniform(uniforms, "SpaceObjects"), &activeObjectsBuffer };
    }

    app.renderer.AddRenderStage(app, app.mainRenderChain, &pointLightSpriteStage, &RenderPointLightSprites);
    // app.renderer.AddRenderStage(app, app.mainRenderChain, &universeActiveStage, &RenderActiveUniverses);
    // app.renderer.AddRenderStage(app, app.mainRenderChain, &cosmicWebMatterStage, &RenderCosmicWebMatter, this);
    // app.renderer.AddRenderStage(app, app.renderer.renderChain, &gbufferSolidsStage, &RenderGBufferSolids);
}



void SpaceEngine::SetupBuffers(Application& app){
    HXStorageResourceConfig pls_buff_config{};
    pls_buff_config.Data = NULL;
    pls_buff_config.Usage = HX_GRAPHICS_USAGE_GPU_R | HX_GRAPHICS_USAGE_CPU_R | HX_GRAPHICS_USAGE_CPU_W;
    pls_buff_config.Type = HX_GRAPHICS_UNIFORMBUFFER;
    pls_buff_config.Width = sizeof(PointLightSpriteObjGPU) * MAX_UBO_PLS_COUNT;
    pls_buff_config.Output = &pointLightSpritesObjBuffer;

    HXStorageResourceConfig inactobj_buff_config = pls_buff_config;
    inactobj_buff_config.Width = sizeof(InactiveSpaceObjectGPU) * MAX_UBO_SPC_OBJ_STRUCT_COUNT;
    inactobj_buff_config.Output = &inactiveObjectsBuffer;

    HXStorageResourceConfig actobj_buff_config = pls_buff_config;
    actobj_buff_config.Width = sizeof(SpaceObjectGPU) * MAX_UBO_SPC_OBJ_STRUCT_COUNT;
    actobj_buff_config.Output = &activeObjectsBuffer;

    HXStorageResourceConfig csmhalo_buff_config{};
    csmhalo_buff_config.Data = NULL;
    csmhalo_buff_config.Usage = HX_GRAPHICS_USAGE_GPU_R | HX_GRAPHICS_USAGE_GPU_W;
    csmhalo_buff_config.Type = HX_GRAPHICS_STORAGEBUFFER;
    csmhalo_buff_config.Width = sizeof(float) * COSMICWEB_DENSITY_VIEW_CELL_SIZE*COSMICWEB_DENSITY_VIEW_CELL_SIZE*COSMICWEB_DENSITY_VIEW_CELL_SIZE;
    csmhalo_buff_config.Output = &cosmicWebPointClouds;

    app.hxg.CreateStorageResources(
        HXCommandExecutionState{}, pls_buff_config, inactobj_buff_config, actobj_buff_config, csmhalo_buff_config
    );
    app.hxg.MapStorageBuffer(pointLightSpritesObjBuffer, 0, 0, (void**)(&pointLightSpritesObjBufferPtr));
    app.hxg.MapStorageBuffer(inactiveObjectsBuffer, 0, 0, (void**)(&inactiveObjectsBufferPtr));
    app.hxg.MapStorageBuffer(activeObjectsBuffer, 0, 0, (void**)(&activeObjectsBufferPtr));

    app.hxg.CreateVertexBufferView(cosmicWebPointCloudsView, HXVBufferDesc {
        &cosmicWebPointClouds, sizeof(float), 0
    });
}



void SpaceEngine::SetupTextures(Application& app){

}




void SpaceEngine::SetupComputePipelines(Application& app){


}



void SpaceEngine::Start(Application& app){
    CreateHornTorus(app);
    CreateIcoSphere(app);
    SetupBuffers(app);
    SetupTextures(app);
    SetupPipelines(app);
    SetupComputePipelines(app);

    cosmicWebBlock.Init(app, *this);
}



void SpaceEngine::PreUpdate(Application& app){
    if (observer.camera){
        observer.position.x = observer.camera->eye.x;
        observer.position.y = observer.camera->eye.y;
        observer.position.z = observer.camera->eye.z;
    }

    pointLightSpritesObjBufferCounter = 0;
    inactiveObjectsBufferCounter = 0;
    activeObjectsBufferCounter = 0;
    pls_obj_range.zero(0);

    MultiverseWorldConstants gpu_world{};
    gpu_world.up_time = (float)(app.up_time * 0.001);
    memcpy(app.renderer.worldBufferPtr, &gpu_world, sizeof(MultiverseWorldConstants));

    cosmicWebBlock.Update(app, *this);
}



void SpaceEngine::PostUpdate(Application& app){
    universeActiveStage.uniforms[2].Range.Start = active_universes_obj_range.start * sizeof(SpaceObjectGPU);
    universeActiveStage.uniforms[2].Range.Size = active_universes_obj_range.count * sizeof(SpaceObjectGPU);

    pointLightSpriteStage.uniforms[1].Range.Start = pls_obj_range.start * sizeof(PointLightSpriteObjGPU);
    pointLightSpriteStage.uniforms[1].Range.Size = pls_obj_range.count * sizeof(PointLightSpriteObjGPU);

    // cosmicWebMatterStage.uniforms[1].Range.Start = cosmicweb_matter_obj_range.start * sizeof(InactiveSpaceObjectGPU);
    // cosmicWebMatterStage.uniforms[1].Range.Size = cosmicweb_matter_obj_range.count * sizeof(InactiveSpaceObjectGPU);
}



void SpaceEngine::CleanUp(Application& app){

}



SpaceObjectGPU SpaceEngine::UpdateSpaceObject(
    SpaceObject* obj, const SpaceObject* parent, spc_dvec3& accumPos, spc_dvec3& rel_position
){
    double parent_scale = 1.0;
    spc_dvec3 parent_pos = spc_dvec3(0);
    quat parent_rot = vec3(0.0, 0.0, 0.0);

    if (parent){
        // parent_scale = parent->radius / SPC_PARENT_UNIT_SCALE;
        parent_pos = parent->accumPos;
        parent_rot = parent->rotation;
    }

    spc_dvec3 objpos = obj->position;// * parent_scale;
    rel_position = spc_dvec3(
        objpos.x - (observer.position.x-parent_pos.x),
        objpos.y - (observer.position.y-parent_pos.y),
        objpos.z - (observer.position.z-parent_pos.z)
    );

    accumPos = objpos + parent_pos;
    obj->accumPos = accumPos;
    obj->rotation *= parent_rot;
    obj->rotationInv = Mathgl::inverse(obj->rotation);


    // scale for the gpu
    SpaceObjectGPU gpu{};

    // the reason for dividing the relative position with the exponent is so that
    // the bigger the object, the closer the gpu position will be so that you won't see
    // jittering on really large objects at really far distances
    // but now the jittering is left for really tiny objects, too tiny to even notice jittering
    const uint radius_exponent = StaticUtils::double_extract_exponent(obj->radius);
    gpu.position = vec3(StaticUtils::double_extract_mantissa(rel_position, radius_exponent) / obj->scale);
    gpu.rotation = vec4(obj->rotation.x, obj->rotation.y, obj->rotation.z, obj->rotation.w);
    gpu.rotationInv = vec4(obj->rotationInv.x, obj->rotationInv.y, obj->rotationInv.z, obj->rotationInv.w);
    // gpu.scale = obj->scale;

    // normalize it to just below a 16 bit float mantissa bits so we use at least half of precision of the entire float mantissa
    // we dont know how many bits the actual depth of each triangle will need to use, so to be safe we normalize to 16 bit float
    gpu.rel_pos_div_radius = vec3(rel_position/obj->radius);
    // gpu.true_radius = float(obj->radius/8388608.0); // 2^23 (32 bit float mantissa bits)
    gpu.true_radius = float(StaticUtils::double_extract_mantissa(obj->radius, radius_exponent)); // 2^23 (32 bit float mantissa bits)

    // std::cout << "length for " << i << " = " << gpu.true_depth << '\n';

    return gpu;
}



InactiveSpaceObjectGPU SpaceEngine::UpdateInactiveSpaceObject(
    SpaceObject* obj, const SpaceObject* parent, spc_dvec3& accumPos, spc_dvec3& rel_position
){
    double parent_scale = 1.0;
    spc_dvec3 parent_pos = spc_dvec3(0);
    quat parent_rot = vec3(0.0, 0.0, 0.0);

    if (parent){
        // parent_scale = parent->radius / SPC_PARENT_UNIT_SCALE;
        parent_pos = parent->accumPos;
        parent_rot = parent->rotation;
    }

    spc_dvec3 objpos = obj->position;// * parent_scale;
    rel_position = spc_dvec3(
        objpos.x - (observer.position.x-parent_pos.x),
        objpos.y - (observer.position.y-parent_pos.y),
        objpos.z - (observer.position.z-parent_pos.z)
    );

    accumPos = objpos + parent_pos;
    obj->accumPos = accumPos;
    obj->rotation *= parent_rot;
    obj->rotationInv = Mathgl::inverse(obj->rotation);


    // scale for the gpu
    InactiveSpaceObjectGPU gpu{};
    const uint radius_exponent = StaticUtils::double_extract_exponent(obj->radius);
    gpu.position = vec3(StaticUtils::double_extract_mantissa(rel_position, radius_exponent) / obj->scale);
    gpu.rotation = vec4(obj->rotation.x, obj->rotation.y, obj->rotation.z, obj->rotation.w);
    gpu.rotationInv = vec4(obj->rotationInv.x, obj->rotationInv.y, obj->rotationInv.z, obj->rotationInv.w);
    return gpu;
}



void SpaceEngine::SetObserver(const Observer& observer){
    this->observer = observer;
}








void ComputeCosmicWebDensity(RSI_RENDERSTAGE_PARAMS){
    const uint texsize = COSMICWEB_DENSITY_VIEW_CELL_SIZE;
    const uint workGroupSize = 8;
    HXDispatchComputeWork dispatch{};
    dispatch.x = size_t(ceil(texsize / workGroupSize));
    dispatch.y = size_t(ceil(texsize / workGroupSize));
    dispatch.z = size_t(ceil(texsize / workGroupSize));
    app->hxg.InsertCommands(cmds_state, dispatch);
}
void RenderCosmicWebDensity(RSI_RENDERSTAGE_PARAMS){
    const uint workGroupSize = 32;
    HXDispatchComputeWork dispatch{};
    dispatch.x = size_t(ceil(double(app->mainWindow.screenSize.x) / workGroupSize));
    dispatch.y = size_t(ceil(double(app->mainWindow.screenSize.y) / workGroupSize));
    dispatch.z = 1;
    app->hxg.InsertCommands(cmds_state, dispatch);
}
void CosmicWebBlockRenderer::Init(Application& app, SpaceEngine& engine){
    {
        cosmicWebDensityGen.active = true;
        cosmicWebDensityGen.type = HX_GRAPHICS_CMDBUFFER_COMPUTE;
        cosmicWebDensityGen.pipeline = app.StrMapGet("cosmicWebDensityGen");
        HXShaderUniformMap& uniforms = app.renderer.registry.computePipelines[HX_NULL_ID_GET_ID(cosmicWebDensityGen.pipeline)].uniforms;
        cosmicWebDensityGen.uniforms[0] = HXShaderInput{ app.hxg.GetUniform(uniforms, "outputImage"), &cosmicWebDenityTex };
        app.renderer.AddRenderStage(app, app.initComputeChain, &cosmicWebDensityGen, &ComputeCosmicWebDensity);
    }; {
        cosmicWebDensityDraw.active = true;
        cosmicWebDensityDraw.type = HX_GRAPHICS_CMDBUFFER_COMPUTE;
        cosmicWebDensityDraw.pipeline = app.StrMapGet("cosmicWebDensityDraw");
        HXShaderUniformMap& uniforms = app.renderer.registry.computePipelines[HX_NULL_ID_GET_ID(cosmicWebDensityDraw.pipeline)].uniforms;
        cosmicWebDensityDraw.uniforms[0] = HXShaderInput{
            app.hxg.GetUniform(uniforms, "outputImage"),
            &app.renderer.registry.renderTargets[HX_NULL_ID_GET_ID(app.StrMapGet("transparentObjectsColorRT"))].tex
        };
        cosmicWebDensityDraw.uniforms[1] = HXShaderInput{ app.hxg.GetUniform(uniforms, "CameraData"), &app.renderer.cameraBuffer };
        cosmicWebDensityDraw.uniforms[2] = HXShaderInput{ app.hxg.GetUniform(uniforms, "SpaceObject"), &spcobj };
        cosmicWebDensityDraw.uniforms[3] = HXShaderInput{ app.hxg.GetUniform(uniforms, "cosmicWebDenityTex"), &cosmicWebDenityTex, 1, &app.renderer.mipmapSampler };
        cosmicWebDensityDraw.uniforms[4] = HXShaderInput{ app.hxg.GetUniform(uniforms, "blueNoiseTex"), &app.renderer.blueNoiseTex, 1, &app.renderer.linearSampler };
        app.renderer.AddRenderStage(app, app.updateComputeChain, &cosmicWebDensityDraw, &RenderCosmicWebDensity);
    }

    HXStorageResourceConfig cosmicweb_tex_config{};
    cosmicweb_tex_config.Data = NULL;
    cosmicweb_tex_config.Type = HX_GRAPHICS_TEXTURE_3D;
    cosmicweb_tex_config.Usage = HX_GRAPHICS_USAGE_GPU_R | HX_GRAPHICS_USAGE_GPU_W;
    cosmicweb_tex_config.Width = COSMICWEB_DENSITY_VIEW_CELL_SIZE;
    cosmicweb_tex_config.Height = cosmicweb_tex_config.Width;
    cosmicweb_tex_config.Depth = cosmicweb_tex_config.Width;
    cosmicweb_tex_config.Format = HX_R8_UNORM;
    cosmicweb_tex_config.Output = &cosmicWebDenityTex;
    app.hxg.CreateStorageResources(HXCommandExecutionState{}, cosmicweb_tex_config);

    cosmicWebBlock.shaderStage.type = HX_GRAPHICS_CMDBUFFER_GRAPHICS;
    cosmicWebBlock.shaderStage.clearColor = vec4(0,0,0,0);
    cosmicWebBlock.shaderStage.renderPassFlags = HX_GRAPHICS_RENDERPASS_COLOUR;
    cosmicWebBlock.shaderStage.clearFlags = 0;
    cosmicWebBlock.shaderStage.outputTextures[0] = app.StrMapGet("gbufferColorRT");
    cosmicWebBlock.shaderStage.pipeline = app.StrMapGet("cosmicWebMatterPipeline");
    cosmicWebBlock.shaderStage.depthStencil = NULL;

    {
        HXShaderUniformMap& uniforms = app.renderer.registry.graphicsPipelines[HX_NULL_ID_GET_ID(cosmicWebBlock.shaderStage.pipeline)].uniforms;
        cosmicWebBlock.shaderStage.uniforms[3] = HXShaderInput{
            app.hxg.GetUniform(uniforms, "cosmicWebDenityTex"), &cosmicWebDenityTex, 1, &app.renderer.mipmapSampler
        };
    }
    cosmicWebBlock.Init(app, engine, app.mainRenderChain, 32, 1);
}



void CosmicWebBlockRenderer::Update(Application& app, SpaceEngine& engine){
    spc_dvec3 accumPos, rel_position;
    spcobj = cosmicWebBlock.Update(app, engine, parent);
}



void CosmicWebBlockRenderer::CleanUp(Application& app, SpaceEngine& engine){

}
