struct Application {
    uint64_t up_time;
    float delta_time;
    uint frame_index;

    GraphicsEngine hxg;
    InputEngine hxi;
    HXInputDevice keyboard;
    HXInputDevice mouse;
    HXInputDevice gamepad;

    Camera mainCamera;
    Window mainWindow;
    RenderPipeline renderer;
    RenderChainID mainRenderChain;
    RenderChainID uiRenderChain;
    RenderChainID initComputeChain;
    RenderChainID updateComputeChain;
    FontRenderer fontRenderer;
    FontID robotoMono;

    SpaceEngine space;
    Multiverse multiverse;

    bool resizing;
    bool running;
    size_t windowBinding;

    std::unordered_map<std::string, HXUINTP> string_map;
    inline void StrMapStore(const std::string& name, HXUINTP p){ string_map[name] = p; }
    inline HXUINTP StrMapGet(const std::string& name){ return string_map[name]; }


    void Init();
    bool Update();
    void Destroy();
};
