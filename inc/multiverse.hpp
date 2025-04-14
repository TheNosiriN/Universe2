
// In micrometers. One million times the radius of observable universe = 4.399239669e+38
#define SPC_UNIVERSE_RADIUS_MM (1.0e+39)
#define SPC_GALAXY_CLUSTER_RADIUS_MM (1.0e+30)
#define SPC_COSMICWEB_RENDER_RADIUS_MM (1.0e+33)
// #define SPC_UNIVERSE_RADIUS_MM (1000.0)


enum MultiverseObjRenderState : uint8_t {
    SPC_MULTIVERSE_STATE_NONE = 0,
    SPC_MULTIVERSE_STATE_POINTLIGHT = 1,
    SPC_MULTIVERSE_STATE_FAR = 2,
    SPC_MULTIVERSE_STATE_ACTIVE = 3,
};

struct MultiverseObject : SpaceObject {
    bool active;
    uint64_t seed;
    MultiverseObjRenderState renderState;
};


template<typename Obj_t>
struct Cluster : MultiverseObject {
    Utils::TypedVector<Obj_t> objects;

    void Create(Application& app, SpaceEngine& engine);
    void Update(Application& app, SpaceEngine& engine, spc_dvec3 accumPos, const spc_dvec3& c_rel_position);
    void CleanUp(Application& app, SpaceEngine& engine);
};


struct Galaxy : MultiverseObject {
    void Create(Application& app, SpaceEngine& engine);
    void Update(Application& app, SpaceEngine& engine, spc_dvec3 accumPos, const spc_dvec3& c_rel_position);
    void CleanUp(Application& app, SpaceEngine& engine);
};


struct Universe : MultiverseObject {
    Utils::TypedVector<Cluster<Galaxy>> galaxies;
    Utils::TypedVector<float> clusters;

    float cosmicWebGridDensity;
    spc_ivec3 cosmicWebGridIndex;
    InactiveSpaceObjectGPU cosmicWebGridObj;

    void Create(Application& app, SpaceEngine& engine);
    void Update(Application& app, SpaceEngine& engine, spc_dvec3 accumPos, const spc_dvec3& c_rel_position);
    void UpdateClusterGrid(Application& app, SpaceEngine& engine, const spc_dvec3& c_rel_position);
    void UpdateCosmicWebView(Application& app, SpaceEngine& engine, const spc_dvec3& c_rel_position);
    void OnActive(Application& app, SpaceEngine& engine);
    void OnInactive(Application& app, SpaceEngine& engine);
};


struct Multiverse {
    Utils::TypedVector<Universe> universes;

    void Create(Application& app, SpaceEngine& engine);
    void Update(Application& app, SpaceEngine& engine);
    void CleanUp(Application& app, SpaceEngine& engine);

    Universe* AddUniverse(Application& app, SpaceEngine& engine);
    MultiverseObjRenderState GetUniverseRenderState(const Universe& uni, double distance);
};
