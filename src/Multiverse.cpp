#include "main.hpp"
using namespace Hexo::Graphics::Commands;



void Multiverse::Create(Application& app, SpaceEngine& engine){
    AddUniverse(app, engine);
}


void Multiverse::Update(Application& app, SpaceEngine& engine){
    engine.active_universes_obj_range.zero(engine.activeObjectsBufferCounter);
    engine.cosmicweb_matter_obj_range.zero(engine.inactiveObjectsBufferCounter);

    for (size_t i=0; i<universes.size(); ++i){
        Universe& uni = universes[i];
        spc_dvec3 accumPos, rel_position;
        SpaceObjectGPU gpu = engine.UpdateSpaceObject(universes+i, NULL, accumPos, rel_position);
        double distance = length(rel_position);

        auto renderState = GetUniverseRenderState(uni, distance);
        if (uni.renderState != renderState){
            auto prevRenderState = uni.renderState;
            uni.renderState = renderState;
            std::cout << "universe " << i << " switched to state " << size_t(renderState) << '\n';

            if (renderState == SPC_MULTIVERSE_STATE_ACTIVE){
                uni.active = true;
                uni.OnActive(app, engine);
            }else
            if (prevRenderState == SPC_MULTIVERSE_STATE_ACTIVE){
                uni.active = false;
                uni.OnInactive(app, engine);
            }
        }
        if (renderState == SPC_MULTIVERSE_STATE_NONE){ continue; }

        // std::cout << "distance: " << distance << " -- radius: " << uni.radius << '\n';

        switch (renderState){
            case SPC_MULTIVERSE_STATE_ACTIVE: {
                uni.Update(app, engine, accumPos, rel_position);
                hxmemcpy(
                    engine.activeObjectsBufferPtr + engine.active_universes_obj_range.abs_next()*sizeof(SpaceObjectGPU),
                    &gpu, sizeof(SpaceObjectGPU)
                );
                engine.active_universes_obj_range.count += 1;
                engine.activeObjectsBufferCounter += 1;
            } break;

            case SPC_MULTIVERSE_STATE_POINTLIGHT: {
                PointLightSpriteObjGPU pls{};
                pls.position = gpu.position;
                pls.scale = 0.1f;
                hxmemcpy(
                    engine.pointLightSpritesObjBufferPtr + engine.pls_obj_range.abs_next()*sizeof(PointLightSpriteObjGPU),
                    &pls, sizeof(InactiveSpaceObjectGPU)
                );
                engine.pls_obj_range.count += 1;
                engine.pointLightSpritesObjBufferCounter += 1;
            } break;
        }
    }
}


void Multiverse::CleanUp(Application& app, SpaceEngine& engine){

}


Universe* Multiverse::AddUniverse(Application& app, SpaceEngine& engine){
    Universe u = engine.CreateSpaceObject<Universe>(
        spc_dvec3(0,0,0), SPC_UNIVERSE_RADIUS_MM, 1.0f
    );
    u.active = false;

    ivec3 tp = ivec3(u.position.x, u.position.y, u.position.z) + 10;
    // uint32_t seed = uint32_t(tp.x*1973u + tp.y*9277u + tp.z*10195u) | 1u;
    uint32_t seed = StaticUtils::ConstructBasicSeed(ivec4(tp, 0));
    // u.rotation = vec3(0);
    u.rotation = StaticUtils::frandom_quaternion(seed);
    u.rotationInv = Mathgl::inverse(u.rotation);

    u.Create(app, engine);
    universes.push_back(u);
    return universes.data() + universes.last_index();
}

MultiverseObjRenderState Multiverse::GetUniverseRenderState(const Universe& uni, double distance){
    MultiverseObjRenderState state = SPC_MULTIVERSE_STATE_NONE;

    if (distance < uni.radius*12.0){
        state = SPC_MULTIVERSE_STATE_ACTIVE;
    }else{
        state = SPC_MULTIVERSE_STATE_POINTLIGHT;
    }

    return state;
}









void Universe::Create(Application& app, SpaceEngine& engine){
    cosmicWebGridDensity = 0.0;
    cosmicWebGridIndex = spc_ivec3(0);
}


void Universe::OnActive(Application& app, SpaceEngine& engine){
    int gridRadius = 1;
    int gridDiameter = gridRadius * 2 + 1;
    auto totalsize = gridDiameter * gridDiameter * gridDiameter;
    clusters.reserve(totalsize + 1);
    for (int i=0; i<totalsize; clusters.push_back(0.0f), ++i);
}


void Universe::Update(Application& app, SpaceEngine& engine, spc_dvec3 accumPos, const spc_dvec3& c_rel_position){
    UpdateClusterGrid(app, engine, c_rel_position);
    UpdateCosmicWebView(app, engine, c_rel_position);
}


double easeInExpoFac(double x, double fac) {
    return x==0.0 ? 0.0 : pow(fac, 10.0*x - 10.0);
}

double easeInCirc(double x) {
    double s = sqrt(1.0 - (x*x));
    return 1.0 - (isnan(s) ? 0.0 : s);
}

double sdTorus(dvec3 p, dvec2 t, double th){
    dvec2 q = dvec2(length(dvec2(p.x,p.z))-t.x,p.y);
    return max(
        length(q) - t.y,
        -length(q+dvec2(t.y*(1.0-th), 0)) + t.y*th
    );
}

dvec2 getTorusUV(dvec3 p, dvec2 t){
    return dvec2(
        atan(p.z, p.x),
        atan(p.y, length(dvec2(p.x,p.z)) - t.x)
    ) / Mathgl::pi<double>() * 0.5 + 0.5;
}

double makeTorusCol(dvec2 uv){
    dvec2 guv = uv*2.0-1.0;
    double col1 = easeInExpoFac(max(pow(guv.y, 7.0), 0.0), 1.5);
    double col2 = easeInExpoFac(max(pow(-guv.y, 5.0), 0.0), 1.7);
    double col = col1 + col2;
    col *= 1.0-easeInCirc(min(1.3-abs(guv.y+0.2), 1.0));
    return col;
}

double getTorusDensity(dvec3 p, dvec2 torus) {
    double radius = torus.x+torus.y;
    double shape = sdTorus(p, torus, 0.8);
    auto uv = getTorusUV(p, torus);
    double density = makeTorusCol(getTorusUV(p, torus));
    shape = max(smoothstep(radius/30.0, 0.0, shape), 0.0);
    return max(density * shape, 0.0);
}


void Universe::UpdateCosmicWebView(Application& app, SpaceEngine& engine, const spc_dvec3& c_rel_position){
    auto radius = this->radius;
    double gridSize = radius*0.25;//SPC_COSMICWEB_RENDER_RADIUS_MM*2.0; // in millimetres
    dvec2 torus = dvec2(radius*0.5);

    spc_ivec3 newGrid = spc_ivec3(floor(c_rel_position/gridSize + 0.5));
    bool shouldReload = !Mathgl::all(Mathgl::equal(newGrid, cosmicWebGridIndex));
    cosmicWebGridIndex = newGrid;

    double density = 0.0;
    if (shouldReload){
        spc_dvec3 pos = StaticUtils::mulvq(this->rotationInv, spc_dvec3(cosmicWebGridIndex) * gridSize);
        density = getTorusDensity(pos, torus);
        cosmicWebGridDensity = float(density);
    }else{
        density = cosmicWebGridDensity;
    }
    // if (density <= 0.0001){ return; }


    // SpaceObjectGPU gpu = engine.UpdateSpaceObject(&spcobj, this, accumPos, rel_position);

    // InactiveSpaceObjectGPU obj{};
    // obj.position = gpu.position;//vec3(-c_rel_position/this->radius);
    // obj.rotation = vec4(this->rotation.x, this->rotation.y, this->rotation.z, this->rotation.w);
    // obj.rotationInv = vec4(this->rotationInv.x, this->rotationInv.y, this->rotationInv.z, this->rotationInv.w);
    // obj.value = float(density);
    // cosmicWebGridObj = obj;
    // engine.cosmicWebMatterStage.uniforms[1].Data = &cosmicWebGridObj;

    spc_dvec3 accumPos, rel_position;
    SpaceObject spcobj = engine.CreateSpaceObject<SpaceObject>(spc_dvec3(0), gridSize/2.0, 1.0f);
    engine.cosmicWebBlock.SetParent(spcobj);

    // hxmemcpy(
    //     engine.inactiveObjectsBufferPtr + engine.cosmicweb_matter_obj_range.abs_next()*sizeof(InactiveSpaceObjectGPU),
    //     &obj, sizeof(InactiveSpaceObjectGPU)
    // );
    // engine.cosmicweb_matter_obj_range.count += 1;
    // engine.inactiveObjectsBufferCounter += 1;
}


void Universe::UpdateClusterGrid(Application& app, SpaceEngine& engine, const spc_dvec3& c_rel_position){
    int gridRadius = 1;
    auto radius = this->radius;
    double gridSize = SPC_GALAXY_CLUSTER_RADIUS_MM*2.0; // in millimetres
    dvec2 torus = dvec2(radius*0.5);

    // spc_ivec3 newGrid = spc_ivec3(floor(c_rel_position/gridSize + 0.5));
    // bool shouldReload = !Mathgl::all(Mathgl::equal(newGrid, eyeGridIndex));
    // eyeGridIndex = newGrid;

    double eyeGridDensity = 0.0;
    // std::cout << eyeGridIndex.x << " -- " << eyeGridIndex.y << " -- " << eyeGridIndex.z << '\n';

    // for (int x=-gridRadius; x<=gridRadius; ++x){
    //     for (int y=-gridRadius; y<=gridRadius; ++y){
    //         for (int z=-gridRadius; z<=gridRadius; ++z){
    // int x=0, y=0, z=0;
    // spc_dvec3 index = spc_dvec3(eyeGridIndex + spc_ivec3(x,y,z)) * gridSize;
    // int idx = StaticUtils::ind_3Dto1D(ivec3(x,y,z) + gridRadius, ivec3(gridRadius*2+1));
    // double density = 0.0;
    // if (shouldReload){
    //     spc_dvec3 pos = StaticUtils::mulvq(this->rotationInv, index);
    //     density = getTorusDensity(pos, torus);
    //     clusters[idx] = float(density);
    //     if (x==0 && y==0 && z==0){ eyeGridDensity = density; }
    // }else{
    //     density = clusters[idx];
    // }
    // if (density <= 0.0001){ return; }
    //
    // spc_dvec3 accumPos, rel_position;
    // SpaceObject spcobj = engine.CreateSpaceObject<SpaceObject>(-index, gridSize/2.0, 1.0f);
    // SpaceObjectGPU gpu = engine.UpdateSpaceObject(&spcobj, this, accumPos, rel_position);

    // InactiveSpaceObjectGPU pls{};
    // pls.position = gpu.position;
    // pls.rotation = gpu.rotation;
    // pls.rotationInv = gpu.rotationInv;
    // pls.value = float(density);
    // memcpy(
    //     engine.inactiveObjectsBufferPtr + engine.uni_cluster_obj_range.abs_next()*sizeof(InactiveSpaceObjectGPU),
    //     &pls, sizeof(InactiveSpaceObjectGPU)
    // );
    // engine.uni_cluster_obj_range.count += 1;

    //         }
    //     }
    // }


}


void Universe::OnInactive(Application& app, SpaceEngine& engine){
    clusters.clearFast();
}
