struct Application;
struct Window;
struct RenderPipeline;
struct Camera;


#define RSI_RENDERSTAGE_PARAMS Application* app, RenderPipeline* renderer, HXCommandExecutionState& cmds_state, HXUINTP stage, Camera* camera, HXUINTP userdata
typedef void (*RenderStageFunc)(RSI_RENDERSTAGE_PARAMS);

template<int T1, int T2, int U>
struct RenderStage {
    bool active;
    int InputTexCount;
    int OutputTexCount;
    int UniformsCount;

    HXSIZE inputTextures[8];
    HXSIZE outputTextures[8];
    HXSIZE depthStencil;
    HXSIZE pipeline;

    RenderPassFlags renderPassFlags;
    RenderPassFlags clearFlags;
    HXFrameBuffer framebuffer;
    bool noFrameBuffer = false;
    RenderStageFunc renderFunc;
    CommandBufferType type;
    HXUINTP userdata;

    HXTexture internal_inputTextures[8];

    vec2 scale = vec2(1,1);
    vec2 viewSize = vec2(1,1);
    vec4 clearColor;

    HXShaderInput uniforms[U+1];

    RenderStage() : uniforms{}, InputTexCount(T1), OutputTexCount(T2), UniformsCount(U) {}
};



typedef HXSIZE RenderChainID;
struct RenderChain {
    Utils::TypedVector<HXUINTP> array;
    HXCommandBuffer commandBuffer;
    HXGPUFence fence;
};


struct RenderResourceRegistry {
    template<typename View_t>
    struct TexViewPair {
        HXTexture tex;
        View_t view;
        bool dependent;
        uvec2 size;
        bool hdr;
    };

    template<typename Pip_t>
    struct HXPipelinePair {
        Pip_t pipeline;
        HXShaderUniformMap uniforms;
    };

    // TypedVector<HXTexture> textures;
    Utils::TypedVector<TexViewPair<HXRenderTargetView>> renderTargets;
    Utils::TypedVector<TexViewPair<HXDepthStencilView>> depthStencils;
    Utils::TypedVector<HXPipelinePair<HXGraphicsPipeline>> graphicsPipelines;
    Utils::TypedVector<HXPipelinePair<HXComputePipeline>> computePipelines;
};


struct CopyQueueManagerGPU {
    HXSIZE uploadSize = 0;
    HXStorageBuffer uploadBuffer;
    HXUINTP uploadBufferPtr;

    HXSIZE readbackSize = 0;
    HXStorageBuffer readbackBuffer;
    HXUINTP readbackBufferPtr;

    HXSIZE queue_size;
    HXGPUFence fence;
    bool started = false;

    void Init(Application& app);
    void Start(Application& app);
    void Update(Application& app);
    HXUINTP AddReadRequest(Application& app, HXStorageBuffer* buffer, const HXSIZE& offset, const HXSIZE& size);
    HXUINTP AddWriteRequest(Application& app, HXStorageBuffer* buffer, const HXSIZE& offset, const HXSIZE& size);
};


struct BufferedTextureOutput {
    HXGPUFence fence;
    HXSIZE tex;
    HXFrameBuffer framebuffer;
};
template<int C>
using BufferedTextureOutputStream = SwapHelper<BufferedTextureOutput, C>;


struct RenderPipeline {
    RenderResourceRegistry registry;
    RenderStage<3, 1, 0> postStage;
    Utils::TypedVector<RenderChain> chains;
    CopyQueueManagerGPU gpuCopyQueue;

    HXGPUContext globalContext;
    HXCommandBuffer globalCommandBuffer;

    HXGPUContext copyContext;
    HXCommandBuffer copyCommandBuffer;

    HXGPUContext computeContext;
    HXCommandBuffer computeCommandBuffer;

    HXSIZE presentPipeline;
    HXTextureSampler linearSampler;
    HXTextureSampler nearestSampler;
    HXTextureSampler mipmapSampler;

    HXStorageBuffer worldBuffer;
    HXUINTP worldBufferPtr;
    HXStorageBuffer cameraBuffer;
    HXUINTP cameraBufferPtr;

    HXTexture blueNoiseTex;
    HXTexture whiteNoiseTex;


    RenderPipeline();
    void Init(Application& app);
    void PreUpdate(Application& app);
    void PostUpdate(Application& app);
    void Render(Application& app, HXGPUContext& context, RenderChainID chainid, Camera* camera, BufferedTextureOutput& textureStream);
    void Render(Application& app, HXGPUContext& context, RenderChainID chainid);
    void Resize(Application& app, const uvec2& size);
    void Destroy(Application& app);

    void InitCamera(Application& app, Camera& camera);
    RenderChainID CreateRenderChain(Application& app, CommandBufferType type = HX_GRAPHICS_CMDBUFFER_GRAPHICS);
    void StartStageRender(Application& app, RenderStage<0,0,0>* stage, HXCommandExecutionState& cmds_state);
    void EndStageRender(Application& app, RenderStage<0,0,0>* stage, HXCommandExecutionState& cmds_state);
    void AddRenderStage(Application& app, RenderChainID chainid, HXUINTP stage, RenderStageFunc func, HXUINTP data=NULL);
};


struct Camera {
    enum CamType : uint8_t {
        SPC_FLY_CAM_TYPE = 0,
        SPC_ARCBALL_CAM_TYPE,
        SPC_CAM_TYPE_COUNT,
    };

    Camera* parent;

    dvec3 eye;
    double velocity;
    double acceleration;
    CamType cam_type;

    mat4 projection;
	mat4 view;
    float fov;
    float near_plane;
    uvec2 resolution;

    static constexpr int OutputStreamTexCount = 2;
    BufferedTextureOutputStream<OutputStreamTexCount> outputStream;

// private:
    bool mouseLocked;

    dvec3 real_cam_angle;
    dvec3 cam_angle;

    vec3 real_forward_dir;
    vec3 real_left_dir;
    vec3 forward_dir;
    vec3 left_dir;
    float forward_spd;
    float left_spd;

public:
    void Init(Application& app, const vec3& position, float fov, const uvec2& size);
    void Update(Application& app);
    void UpdateArcballCamera(Application& app);
    void UpdateFreeMoveCamera(Application& app);
    void Resize(Application& app, const uvec2& size);
    void Destroy(Application& app);
};


struct Window {
    HXWindow internal;
    uvec2 screenSize;
    float screenRatio;
    HXGPUContext* context;
    HXRenderTargetView rtViews[HX_GRAPHICS_DEF_FRAME_COUNT];
    HXFrameBuffer framebuffer[HX_GRAPHICS_DEF_FRAME_COUNT];
    HXCommandBuffer commandBuffer;

    void Init(Application& app, const HXWindowConfig& config);
    void Render(Application& app, BufferedTextureOutput& finalTexture);
    void Resize(Application& app, const uvec2& size);
    void Destroy(Application& app);
    HXGPUContext& GetContext(Application& app);
};


typedef HXSIZE FontID;
struct Font {
    HXTexture atlasTexture;
    uint charTexSize;
    HXStorageBuffer glypsBuffer;
    HXStorageBuffer textBuffer;
    HXUINTP textBufferPtr;
    FontWindowConstants window;
    float advances[256];
    vec4 offsets[256];
    uint charCounter;
    uint encoding;
    bool Init(Application& app, const std::string& filename, uint encoding, HXCommandExecutionState& cmd_state);
};

struct FontRenderer {
    RenderStage<0,1,4> textRenderStage;
    Utils::TypedVector<Font> fonts;

    void Init(Application& app);
    void Update(Application& app);
    FontID loadFont(Application& app, const std::string& filename, uint encoding, HXCommandExecutionState cmd_state);
    void RenderText(Application& app, const FontID& fontid, const vec2& position, const std::string& text, const float& scale);
    void CleanUp(Application& app);
};
