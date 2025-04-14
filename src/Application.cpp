#include "main.hpp"


void InitEngines(Application& app){
    app.hxg.enableValidationLayer = true;
    app.hxg.InitInThread();

    KeyMappingDesc kdesc[RSI_BUTTON_COUNT];
    hxmemset(kdesc, 0, sizeof(kdesc));
    kdesc[RSI_QUIT_BUTTON].bindings[0] = HX_INPUT_VK_ESCAPE;
    kdesc[RSI_LMB_BUTTON].bindings[0] = HX_INPUT_VK_LMBUTTON;
    kdesc[RSI_CTRL_BUTTON].bindings[0] = HX_INPUT_VK_CONTROL;
    kdesc[RSI_FWD_BUTTON].bindings[0] = HX_INPUT_VK_CHAR_W;
    kdesc[RSI_LEFT_BUTTON].bindings[0] = HX_INPUT_VK_CHAR_A;
    kdesc[RSI_RIGHT_BUTTON].bindings[0] = HX_INPUT_VK_CHAR_D;
    kdesc[RSI_BCK_BUTTON].bindings[0] = HX_INPUT_VK_CHAR_S;
    kdesc[RSI_BRAKES_BUTTON].bindings[0] = HX_INPUT_VK_SPACE;
    kdesc[RSI_Q_BUTTON].bindings[0] = HX_INPUT_VK_CHAR_Q;
    kdesc[RSI_E_BUTTON].bindings[0] = HX_INPUT_VK_CHAR_E;
    kdesc[RSI_CAM_TYPE_BUTTON].bindings[0] = HX_INPUT_VK_CHAR_C;

    kdesc[RSI_FWD_BUTTON].bindings[1] = HX_INPUT_VK_UP;
    kdesc[RSI_LEFT_BUTTON].bindings[1] = HX_INPUT_VK_LEFT;
    kdesc[RSI_RIGHT_BUTTON].bindings[1] = HX_INPUT_VK_RIGHT;
    kdesc[RSI_BCK_BUTTON].bindings[1] = HX_INPUT_VK_DOWN;
    kdesc[RSI_CAM_TYPE_BUTTON].bindings[1] = HX_INPUT_VK_PAD_DPAD_UP;
    kdesc[RSI_QUIT_BUTTON].bindings[1] = HX_INPUT_VK_PAD_B;
    kdesc[RSI_BRAKES_BUTTON].bindings[1] = HX_INPUT_VK_PAD_A;

    AnalogMappingDesc adesc[RSI_ANALOGUE_COUNT];
    hxmemset(adesc, 0, sizeof(adesc));
    adesc[RSI_MOUSE_POS].binding = HX_INPUT_AN_MOUSE_ABSOLUTE_POS;
    adesc[RSI_MOUSE_DELTA_ANALOGUE].binding = HX_INPUT_AN_MOUSE_DELTA_POS;
    adesc[RSI_MOUSE_WHEEL_ANALOGUE].binding = HX_INPUT_AN_MOUSE_WHEEL_DELTA;
    adesc[RSI_LSTICK_ANALOGUE].binding = HX_INPUT_AN_PAD_LSTICK;
    adesc[RSI_RSTICK_ANALOGUE].binding = HX_INPUT_AN_PAD_RSTICK;

    InputEngineDesc desc{};
    desc.Keymaps = kdesc;
    desc.Analogmaps = adesc;
    desc.KeymapsCount = sizeof(kdesc)/sizeof(KeyMappingDesc);
    desc.AnalogmapsCount = sizeof(adesc)/sizeof(AnalogMappingDesc);
    desc.NumberOfMice = 1;
    desc.NumberOfGamepads = 1;
    desc.NumberOfKeyboards = 1;

    app.hxi.InitInThread(desc);
    app.keyboard = app.hxi.GetKeyboardDevice(0);
    app.mouse = app.hxi.GetMouseDevice(0);
    app.gamepad = app.hxi.GetGamepadDevice(0);
}


void RenderPostProcess(RSI_RENDERSTAGE_PARAMS){
    using namespace Hexo::Graphics::Commands;
    HXVertexDraw vdraw{};
    vdraw.StartVertex = 0;
    vdraw.StartInstance = 0;
    vdraw.VertexCount = 6;
    vdraw.InstanceCount = 1;
    vdraw.VDesc = NULL;
    vdraw.BufferCount = 0;
    app->hxg.InsertCommands(cmds_state, vdraw);
}



void Application::Init(){
    InitEngines(*this);

    resizing = false;
    running = true;

    // create a new window
    uvec2 screenSize = uvec2(1280, 720);
    HXWindowConfig winconfig{ "The RSI Multiverse Engine", screenSize, false, 60, false, true };
    winconfig.OnResize = [&](GraphicsEngine*, HXWindow, uvec2 oldSize, uvec2 newSize){
        resizing = true;
        hxg.FlushContexts();
        renderer.Resize(*this, newSize);
        mainCamera.Resize(*this, newSize);
        mainWindow.Resize(*this, newSize);
        resizing = false;
        return HXRC_OK;
    };

    winconfig.OnDestroy = [&](GraphicsEngine*, HXWindow){
        running = false;
        return HXRC_OK;
    };

    renderer.Init(*this);
    mainRenderChain = renderer.CreateRenderChain(*this, HX_GRAPHICS_CMDBUFFER_GRAPHICS);
    initComputeChain = renderer.CreateRenderChain(*this, HX_GRAPHICS_CMDBUFFER_COMPUTE);
    updateComputeChain = renderer.CreateRenderChain(*this, HX_GRAPHICS_CMDBUFFER_COMPUTE);

    mainWindow.Init(*this, winconfig);
    windowBinding = hxi.AddWindowBinding(hxg.GetNativeWindowHandle(mainWindow.internal));

    fontRenderer.Init(*this);

    // load fonts
    HXCommandExecutionState font_config_state{};
    font_config_state.Context = &renderer.copyContext;
    font_config_state.CommandBuffer = &renderer.copyCommandBuffer;
    font_config_state.StartCommandBuffer = true;
    font_config_state.EndCommandBuffer = true;
    robotoMono = fontRenderer.loadFont(*this, "RobotoMono", 8, font_config_state);

    hxg.ExecuteCommandBuffers(renderer.copyContext, NULL, renderer.copyCommandBuffer);
    hxg.WaitForExecution(renderer.copyContext);
    hxg.CleanUpResourceCopyStates(font_config_state);
    //

    memset(&mainCamera, 0, sizeof(mainCamera));
    mainCamera.Init(*this, vec3(0,0,0), 30.0, screenSize);

    // start space engine
    space.Start(*this);
    multiverse.Create(*this, space);

    Observer player{};
    player.camera = &mainCamera;
    space.SetObserver(player);

    renderer.AddRenderStage(*this, mainRenderChain, &renderer.postStage, &RenderPostProcess);
    renderer.Render(*this, renderer.computeContext, initComputeChain);

    renderer.Resize(*this, screenSize);
    mainCamera.Resize(*this, screenSize);


    this->up_time = 0;
    this->delta_time = 0;
    this->frame_index = 0;

    hxg.WaitForExecution(renderer.computeContext);
}



bool Application::Update(){
    ClockTimer<std::chrono::microseconds> timer{};
    timer.start();

    if (
        hxi.GetKey(gamepad, RSI_QUIT_BUTTON, HX_INPUT_KEYSTATE_RELEASED) ||
        hxi.GetKey(keyboard, RSI_QUIT_BUTTON, HX_INPUT_KEYSTATE_RELEASED)
    ){
        running = false;
        return running;
    }
    if (resizing)return running;

    renderer.PreUpdate(*this);

    fontRenderer.Update(*this);
    mainCamera.Update(*this);

    space.PreUpdate(*this);
    multiverse.Update(*this, space);
    space.PostUpdate(*this);

    renderer.PostUpdate(*this);

    renderer.Render(*this, renderer.computeContext, updateComputeChain);
    renderer.Render(*this, renderer.globalContext, mainRenderChain, &mainCamera, mainCamera.outputStream.current());
    mainWindow.Render(*this, mainCamera.outputStream.previous());
    mainCamera.outputStream.advance();

    hxi.Update();
    hxg.Update();
    hxg.UpdateInThread();
    hxg.PresentToWindow(mainWindow.internal);

    timer.stop();
    this->delta_time = timer.duration().count() * 0.001f;
    this->up_time += (uint64_t)this->delta_time;
    this->frame_index += 1;
    return running;
}



void Application::Destroy(){
    hxg.FlushContexts();

    mainCamera.Destroy(*this);
    mainWindow.Destroy(*this);
    renderer.Destroy(*this);

    hxg.Release();
    hxi.Release();
}
