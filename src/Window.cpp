#include "main.hpp"
using namespace Hexo::Graphics::Commands;


void Window::Init(Application& app, const HXWindowConfig& config){
    screenSize = config.Size;
    screenRatio = float(config.Size.x) / float(config.Size.y);
    app.hxg.CreateNewWindow(internal, config);
    context = app.hxg.GetWindowGPUContext(internal);

    for (int i=0; i<HX_GRAPHICS_DEF_FRAME_COUNT; ++i){
        rtViews[i] = app.hxg.GetWindowRenderTarget(internal, i);
        HXFrameBufferConfig rpconfig{};
        rpconfig.Flags = HX_GRAPHICS_RENDERPASS_COLOUR;
        rpconfig.ClearFlags = 0;
        rpconfig.RenderTargets[0] = rtViews+i;
        rpconfig.RenderTargetCount = 1;
        rpconfig.DepthStencil = NULL;
        rpconfig.ClearColor = vec4(0,0,0,0);
        app.hxg.CreateFrameBuffer(rpconfig, framebuffer[i]);
    }

    HXCommandBufferConfig win_cmds_config{};
    win_cmds_config.Type = HX_GRAPHICS_CMDBUFFER_GRAPHICS;
    win_cmds_config.Usage = HX_GRAPHICS_CMDBUFFER_ONCE;
    app.hxg.CreateCommandBuffer(commandBuffer, win_cmds_config);
}



void Window::Render(Application& app, BufferedTextureOutput& textureStream){
    HXTexture backbuffer = app.hxg.GetCurrentWindowTexture(internal);

    HXTexture& finalTexture = app.renderer.registry.renderTargets[
        HX_NULL_ID_GET_ID(textureStream.tex)
    ].tex;
    auto& pip_pair = app.renderer.registry.graphicsPipelines[
        HX_NULL_ID_GET_ID(app.renderer.presentPipeline)
    ];
    HXGraphicsPipeline& pipeline = pip_pair.pipeline;

    HXShaderInput uniforms_list[]{
        { app.hxg.GetUniform(pip_pair.uniforms, "ScreenTexture"), &finalTexture, 1, &app.renderer.nearestSampler }
    };
    uniforms_list[0].Usage = HX_GRAPHICS_SHADER_INPUT_USAGE_DYNAMIC;
    HXShaderInputsDesc uniforms{};
    uniforms.Inputs = uniforms_list;
    uniforms.Count = sizeof(uniforms_list)/sizeof(HXShaderInput);

    // create commands
    HXChangeResourceState barr_cmd{};
    barr_cmd.state = HX_GRAPHICS_RESOURCE_STATE_RENDER_TARGET;
    barr_cmd.beforeState = 0;
    barr_cmd.resource = &backbuffer;

    HXChangeResourceState tex_barr_cmd{};
    tex_barr_cmd.state = HX_GRAPHICS_RESOURCE_STATE_SHADER_RESOURCE;
    tex_barr_cmd.beforeState = 0;
    tex_barr_cmd.resource = &finalTexture;

    HXSetGraphicsPipeline setpip_cmd{};
    setpip_cmd.pipeline = &pipeline;
    setpip_cmd.uniforms = uniforms;

    HXStartRenderPass rp_cmd{};
    rp_cmd.framebuffer = framebuffer + app.hxg.GetFrameIndex(internal);
    rp_cmd.viewport = vec4(0, 0, screenSize);

    HXVertexDraw vdraw{};
    vdraw.StartVertex = 0;
    vdraw.StartInstance = 0;
    vdraw.VertexCount = 6;
    vdraw.InstanceCount = 1;
    vdraw.VDesc = NULL;
    vdraw.BufferCount = 0;

    HXEndRenderPass end_rp_cmd{};

    HXChangeResourceState barr_cmd2{};
    barr_cmd2.state = HX_GRAPHICS_RESOURCE_STATE_WINDOW_PRESENT;
    barr_cmd2.beforeState = 0;
    barr_cmd2.resource = &backbuffer;

    // record window draw commands
    HXCommandExecutionState win_cmds_state{};
    win_cmds_state.CommandBuffer = &commandBuffer;
    win_cmds_state.Context = context;
    win_cmds_state.StartCommandBuffer = true;
    win_cmds_state.EndCommandBuffer = true;

    // wait for texture stream to be available
    app.hxg.FenceWaitGPU(*context, textureStream.fence);
    app.hxg.StartRecording(win_cmds_state);
    app.hxg.InsertCommands(win_cmds_state,
        barr_cmd,
        tex_barr_cmd,

        setpip_cmd,
        rp_cmd,
        vdraw,
        end_rp_cmd,

        barr_cmd2
    );
    app.hxg.StopRecording(win_cmds_state);
    app.hxg.ExecuteCommandBuffers(*context, NULL, commandBuffer);
    app.hxg.WaitForExecution(*context);
    app.hxg.ResetGPUContext(*context);
    // app.hxg.ResetCommandBuffer(commandBuffer);
}



void Window::Resize(Application& app, const uvec2& size){
    screenSize = size;
    screenRatio = float(size.x) / float(size.y);
    app.hxg.ResetCommandBuffer(commandBuffer);
}



void Window::Destroy(Application& app){
    app.hxg.DestroyWindow(internal);
}


HXGPUContext& Window::GetContext(Application& app){
    return *app.hxg.GetWindowGPUContext(internal);
}
