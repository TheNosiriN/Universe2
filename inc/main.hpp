#include <iostream>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <chrono>

#include "HexoInput.h"
#include "HexoGraphics.h"
#include "HexoFileSystem.h"

// #ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "stb_image.h"
#include "json.hpp"

using namespace Hexo;
using namespace Hexo::Mathgl;
using namespace Hexo::Graphics;
using namespace Hexo::ShaderC;
using namespace Hexo::Input;

#include "utils.hpp"
#include "inc/spc_coord.hpp"

#define __CONVERT_SHADER_HEADER
#include "common/shader_globals.inl.glsl"
#include "common/font_globals.inl.glsl"
#include "common/part_octree_globals.inl.glsl"
#include "common/static_part_block_globals.inl.glsl"
#include "structs.hpp"
#include "space_engine.hpp"
#include "multiverse.hpp"
#include "application.hpp"


enum : uint8_t {
    RSI_QUIT_BUTTON = 0,
    RSI_LMB_BUTTON,
    RSI_CTRL_BUTTON,
    RSI_FWD_BUTTON,
    RSI_LEFT_BUTTON,
    RSI_RIGHT_BUTTON,
    RSI_BCK_BUTTON,
    RSI_BRAKES_BUTTON,
    RSI_Q_BUTTON,
    RSI_E_BUTTON,
    RSI_CAM_TYPE_BUTTON,

    RSI_BUTTON_COUNT,
};


enum : uint8_t {
    RSI_MOUSE_POS = 0,
    RSI_MOUSE_DELTA_ANALOGUE,
    RSI_MOUSE_WHEEL_ANALOGUE,
    RSI_LSTICK_ANALOGUE,
    RSI_RSTICK_ANALOGUE,

    RSI_ANALOGUE_COUNT,
};
