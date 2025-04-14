#define SPC_PARENT_UNIT_SCALE (4.5035996e15) /* 2^52, 52 = number of bit of double mantissa */




struct spc_ivec3_big {
    using size_type = spc_coord_256_64_t;
    size_type x;
    size_type y;
    size_type z;

    spc_ivec3_big() : x(), y(), z() {}
    spc_ivec3_big(
        const size_type& _x,
        const size_type& _y,
        const size_type& _z
    ){
        x = _x;
        y = _y;
        z = _z;
    }
    spc_ivec3_big(const double& _x, const double& _y, const double& _z) {
        x = _x;
        y = _y;
        z = _z;
    }
};

typedef i64vec3 spc_ivec3;
typedef i64vec2 spc_ivec2;
typedef dvec3 spc_dvec3;
typedef dvec2 spc_dvec2;
typedef double spc_float;
typedef size_t SpaceObjectID;
typedef int64_t spc_int;

struct SpaceEngine;
struct SpaceObject;
struct UnboundedSpaceObject;
struct Observer;
struct ParticleOctree;



struct SpaceObject {
    spc_dvec3 accumPos;
    spc_dvec3 position;
    double radius;
    float scale = 1;
    quat rotation;
    quat rotationInv;
};

struct UnboundedSpaceObject {
    spc_ivec3_big position;
    double radius;
    float scale = 1;
};


struct Observer {
    spc_dvec3 position;
    // spc_ivec3_big position;
    Camera* camera;
};



struct ParticleOctree {
    RenderChainID computeChain;
    RenderStage<0,0,3> computeStage;
    HXStorageBuffer leafNodes;
    HXStorageBuffer atomicCounters;

    bool shouldUpdate;
    spc_dvec3 radius;
    uint curMaxDepth;
    uint curParticleCount;
    ParticleOctreeProps properties;

    void Init(
        Application& app, SpaceEngine& engine, const RenderChainID& chain,
        const HXSIZE& pipeline, const uvec3& startGrid, const uint& maxDepth
    );
    void Update(Application& app, SpaceEngine& engine);
    void CleanUp(Application& app, SpaceEngine& engine);
    void RebuildGrid(Application& app);

    template<typename T>
    void UpdateSpaceObject(const T& obj){
        properties.position = -obj.position;//floor(obj.position * properties.minGridSize);
        properties.rotation = obj.rotation;
        properties.rotationInv = obj.rotationInv;
    }
};



struct StaticParticlesBlock {
    RenderStage<0, 1, 4> shaderStage;
    HXStorageBuffer relposBuffer;
    HXUINTP relposBufferPtr;
    StaticParticlesBlockProps properties;

    void Init(
        Application& app, SpaceEngine& engine, const RenderChainID& chain,
        const uint& gridSize, const uint& levels
    );
    InactiveSpaceObjectGPU Update(Application& app, SpaceEngine& engine, const SpaceObject& parent);
    void CleanUp(Application& app, SpaceEngine& engine);
};



struct CosmicWebBlockRenderer {
    RenderStage<0, 0, 1> cosmicWebDensityGen;
    RenderStage<0, 0, 5> cosmicWebDensityDraw;
    HXTexture cosmicWebDenityTex;
    StaticParticlesBlock cosmicWebBlock;
    InactiveSpaceObjectGPU spcobj;
    SpaceObject parent;

    void Init(Application& app, SpaceEngine& engine);
    void Update(Application& app, SpaceEngine& engine);
    void CleanUp(Application& app, SpaceEngine& engine);
    void SetParent(const SpaceObject& parent){ this->parent = parent; }
};



struct SpaceEngine {
    Observer observer;

    RenderStage<0, 1, 2> pointLightSpriteStage;
    RenderStage<0, 1, 3> universeFarStage;
    RenderStage<0, 1, 4> universeActiveStage;
    RenderStage<0, 1, 2> gbufferSolidsStage;

    CosmicWebBlockRenderer cosmicWebBlock;

    HXStorageBuffer pointLightSpritesObjBuffer;
    uint pointLightSpritesObjBufferCounter;
    HXUINTP pointLightSpritesObjBufferPtr;

    HXStorageBuffer inactiveObjectsBuffer;
    uint inactiveObjectsBufferCounter;
    HXUINTP inactiveObjectsBufferPtr;

    HXStorageBuffer activeObjectsBuffer;
    uint activeObjectsBufferCounter;
    HXUINTP activeObjectsBufferPtr;

    HXStorageBuffer cosmicWebPointClouds;
    HXVertexBufferViewPack<1> cosmicWebPointCloudsView;

    SpcRange<uint> pls_obj_range;
    SpcRange<uint> active_universes_obj_range;
    SpcRange<uint> cosmicweb_matter_obj_range;


    void Start(Application& app);
    void PreUpdate(Application& app);
    void PostUpdate(Application& app);
    void CleanUp(Application& app);
    void SetupBuffers(Application& app);
    void SetupTextures(Application& app);
    void SetupPipelines(Application& app);
    void SetupComputePipelines(Application& app);
    SpaceObjectGPU UpdateSpaceObject(
        SpaceObject* obj, const SpaceObject* parent, spc_dvec3& accumPos, spc_dvec3& rel_position
    );
    InactiveSpaceObjectGPU UpdateInactiveSpaceObject(
        SpaceObject* obj, const SpaceObject* parent, spc_dvec3& accumPos, spc_dvec3& rel_position
    );
    void SetObserver(const Observer& observer);

    template<typename T = SpaceObject>
    inline T CreateSpaceObject(spc_dvec3 position, double radius, float model_radius){
        T s{};
        s.position = position;
        s.radius = radius;

        // the reason for the scale is to match the positions of 3D model to fit its actual radius in the radius
        // since on the screen it already looks like it matches the true scale of the space object
        // so we only need to resolve the fractional difference in the model's positions and the true scale
        const float radius_mantissa = StaticUtils::double_extract_mantissa(radius, StaticUtils::double_extract_exponent(radius));
        s.scale = radius_mantissa / model_radius;//double_extract_mantissa(obj.model_radius, double_extract_exponent(obj.model_radius));
        s.rotation = vec3(0);
        s.rotationInv = Mathgl::inverse(s.rotation);

        // SpaceObjectID id = objects.push_back(s)+1;
        // objects_order.push_back(id);
        return s;
    }
};






static float cube_vertices[] = {
    -0.5f, -0.5f, -0.5f,    0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,     -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,     -0.5f,  0.5f,  0.5f,

    -0.5f,  0.5f, -0.5f,    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,    -0.5f,  0.5f,  0.5f,
    0.5f, -0.5f, -0.5f,     0.5f,  0.5f, -0.5f,
    0.5f,  0.5f,  0.5f,     0.5f, -0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,     -0.5f, -0.5f,  0.5f,
    0.5f,  0.5f, -0.5f,     -0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f,  0.5f,    0.5f,  0.5f,  0.5f,
};
static uint32_t cube_indices[] = {
    // front and back
    0, 3, 2,        2, 1, 0,
    4, 5, 6,        6, 7 ,4,
    // left and right
    11, 8, 9,       9, 10, 11,
    12, 13, 14,     14, 15, 12,
    // bottom and top
    16, 17, 18,     18, 19, 16,
    20, 21, 22,     22, 23, 20
};


static constexpr float icosphere_phi = Mathgl::pi<float>()/2.0f;
static vec3 icosphere_vertices[12] = {
    vec3(-1,  icosphere_phi, 0), vec3( 1,  icosphere_phi, 0), vec3(-1, -icosphere_phi, 0), vec3( 1, -icosphere_phi, 0),
    vec3(0, -1,  icosphere_phi), vec3(0,  1,  icosphere_phi), vec3(0, -1, -icosphere_phi), vec3(0,  1, -icosphere_phi),
    vec3( icosphere_phi, 0, -1), vec3( icosphere_phi, 0,  1), vec3(-icosphere_phi, 0, -1), vec3(-icosphere_phi, 0,  1)
};

static uint32_t icosphere_indices[60] = {
    0, 11, 5, 0, 5, 1, 0, 1, 7, 0, 7, 10, 0, 10, 11,
    1, 5, 9, 5, 11, 4, 11, 10, 2, 10, 7, 6, 7, 1, 8,
    3, 9, 4, 3, 4, 2, 3, 2, 6, 3, 6, 8, 3, 8, 9,
    4, 9, 5, 2, 4, 11, 6, 2, 10, 8, 6, 7, 9, 8, 1
};


static void ConstructHornTorusMeshVertices(
    uint slices, uint loops, float inner_rad, float outer_rad,
    vec3* vertices, uint* indices, vec2* uvs
){
    for (uint s = 0, i = 0; s <= slices; ++s) {
        float v = (float)s / slices;
        float slice_angle = v * 2 * Mathgl::pi<float>();
        float cos_slices = Mathgl::cos(slice_angle);
        float sin_slices = Mathgl::sin(slice_angle);
        float slice_rad = outer_rad + inner_rad * cos_slices;

        for (uint l = 0; l <= loops; ++l, ++i) {
            //   x=(R+r·cos(v))cos(w)
            //   y=(R+r·cos(v))sin(w)
            //             z=r.sin(v)
            float u = (float)l / loops;
            float loop_angle = u * 2 * Mathgl::pi<float>();
            float cos_loops = Mathgl::cos(loop_angle);
            float sin_loops = Mathgl::sin(loop_angle);

            float x = slice_rad * cos_loops;
            float y = inner_rad * sin_slices;
            float z = slice_rad * sin_loops;

            vertices[i] = vec3(x, -y, z);
            // uvs[i] = vec2(u, v);
        }
    }

    // 0  1  2  3  4  5
    // 6  7  8  9  10 11
    // 12 13 14 15 16 17
    uint vertsPerSlice = loops + 1;
    for (uint i = 0, c = 0; i < slices; ++i) {
        uint v1 = i * vertsPerSlice;
        uint v2 = v1 + vertsPerSlice;

        for (uint j = 0; j < loops; ++j, ++c) {
            indices[c*6 + 0] = v1;
            indices[c*6 + 1] = v1 + 1;
            indices[c*6 + 2] = v2;

            indices[c*6 + 3] = v2;
            indices[c*6 + 4] = v1 + 1;
            indices[c*6 + 5] = v2 + 1;

            v1 += 1;
            v2 += 1;
        }
    }
}
