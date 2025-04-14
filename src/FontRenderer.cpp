#include "main.hpp"


bool Font::Init(Application& app, const std::string& filename, uint encoding, HXCommandExecutionState& cmd_state){
    std::fstream metafile;
    nlohmann::json metadata;
    metafile.open("../assets/fonts/"+filename+"/metadata.json", std::ios_base::in);
    if (metafile.peek() != std::ifstream::traits_type::eof()){
        metadata = nlohmann::json::parse(metafile);
    }else{
        std::cout << "ERROR: Failed to read metadata JSON file: " << (filename+"/metadata.json") << '\n';
        return false;
    }

    const auto& glyphs = metadata["glyphs"];
    const auto& atlas = metadata["atlas"];
    charTexSize = atlas["size"];

    window.distanceRange = float(atlas["distanceRange"])/charTexSize;

    // create storage buffers
    auto atlasTextureData = LoadTexture("../assets/fonts/"+filename+"/atlas.png", atlas["width"], atlas["height"], 4);
    HXStorageResourceConfig atlas_tex_config{};
    atlas_tex_config.Data = atlasTextureData;
    atlas_tex_config.Usage = HX_GRAPHICS_USAGE_GPU_R;
    atlas_tex_config.Type = HX_GRAPHICS_TEXTURE_2D;
    atlas_tex_config.Width = atlas["width"];
    atlas_tex_config.Height = atlas["height"];
    atlas_tex_config.Format = HX_R8_G8_B8_A8_UNORM;
    atlas_tex_config.Output = &atlasTexture;

    HXStorageResourceConfig txt_buff_config{};
    txt_buff_config.Data = NULL;
    txt_buff_config.Usage = HX_GRAPHICS_USAGE_GPU_R | HX_GRAPHICS_USAGE_CPU_R | HX_GRAPHICS_USAGE_CPU_W;
    txt_buff_config.Type = HX_GRAPHICS_UNIFORMBUFFER;
    txt_buff_config.Width = sizeof(RenderTextChar) * FONT_MAX_CHARS;
    txt_buff_config.Output = &textBuffer;

    HXStorageResourceConfig glyph_buff_config{};
    glyph_buff_config.Data = NULL;
    glyph_buff_config.Usage = HX_GRAPHICS_USAGE_GPU_R | HX_GRAPHICS_USAGE_CPU_W;
    glyph_buff_config.Type = HX_GRAPHICS_UNIFORMBUFFER;
    glyph_buff_config.Width = sizeof(GlyphsGPU) * FONT_MAX_GLYPHS;
    glyph_buff_config.Output = &glypsBuffer;

    app.hxg.CreateStorageResources(cmd_state, glyph_buff_config, txt_buff_config, atlas_tex_config);
    std::free(atlasTextureData);

    HXUINTP glypsBufferPtr = NULL;
    app.hxg.MapStorageBuffer(glypsBuffer, 0, 0, (void**)(&glypsBufferPtr));
    app.hxg.MapStorageBuffer(textBuffer, 0, 0, (void**)(&textBufferPtr));
    if (!glypsBufferPtr || !textBufferPtr){
        std::cout << "ERROR: Failed to map gpu buffers" << "\n";
        return false;
    }

    for (uint32_t i=0; i<glyphs.size(); ++i){
        const auto& glyph = glyphs[i];
        uint32_t code = glyph["unicode"];
        if (code > 255)continue;

        advances[code] = float(glyph["advance"]);

        GlyphsGPU gpu{};
        if (glyph.contains("atlasBounds")){
            gpu.atlasBounds = vec4(
                float(glyph["atlasBounds"]["top"]) / atlas["height"],
                float(glyph["atlasBounds"]["right"]) / atlas["width"],
                float(glyph["atlasBounds"]["bottom"]) / atlas["height"],
                float(glyph["atlasBounds"]["left"]) / atlas["width"]
            );

            vec4 planeBounds = vec4(
                float(glyph["planeBounds"]["top"]),
                float(glyph["planeBounds"]["right"]),
                float(glyph["planeBounds"]["bottom"]),
                float(glyph["planeBounds"]["left"])
            );

            float xmin = planeBounds.w; // left
            float ymin = planeBounds.z; // bottom
            float xmax = planeBounds.y; // right
            float ymax = planeBounds.x; // top

            vec2 dimensions = vec2(xmax-xmin, ymax-ymin);
            vec2 pos = vec2(xmin, ymin);
            offsets[code] = vec4(pos, dimensions);

        }else{
            gpu.atlasBounds = vec4(0);
            offsets[code] = vec4(0);
        }

        memcpy(glypsBufferPtr + code*sizeof(GlyphsGPU), &gpu, sizeof(GlyphsGPU));
    }

    // std::cout << metadata.dump(4) << '\n';
    app.hxg.UnmapStorageBuffer(glypsBuffer, 0, 0);
    charCounter = 0;
    return true;
}





void RenderFonts(RSI_RENDERSTAGE_PARAMS){
    auto& textRenderStage = app->fontRenderer.textRenderStage;

    for (int i=0; i<app->fontRenderer.fonts.size(); ++i){
        Font* font = app->fontRenderer.fonts + i;
        if (!font->charCounter)continue;

        font->window.screenRatio = app->mainWindow.screenRatio;
        font->window.screenPxRangeScale = vec2(1.0/app->mainWindow.screenRatio, 1.0) * vec2(app->mainWindow.screenSize);

        textRenderStage.uniforms[0].Data = &font->glypsBuffer;
        textRenderStage.uniforms[1].Data = &font->textBuffer;
        textRenderStage.uniforms[2].Data = &font->atlasTexture;
        textRenderStage.uniforms[3].Data = &font->window;

        Commands::HXSetGraphicsUniforms unicmd{};
        unicmd.uniforms.Inputs = textRenderStage.uniforms;
        unicmd.uniforms.Count = 4;

        Commands::HXVertexDraw vdraw{};
        vdraw.StartVertex = 0;
        vdraw.StartInstance = 0;
        vdraw.VertexCount = 6;
        vdraw.InstanceCount = font->charCounter;
        vdraw.VDesc = NULL;
        vdraw.BufferCount = 0;

        app->hxg.InsertCommands(cmds_state, unicmd, vdraw);
    }

    // TODO: fix this!! atlasTexture uses the heap
    // textRenderStage.uniforms[0].Data = NULL;
    // textRenderStage.uniforms[1].Data = NULL;
    textRenderStage.uniforms[2].Data = NULL;
}

void FontRenderer::Init(Application& app){
    textRenderStage.type = HX_GRAPHICS_CMDBUFFER_GRAPHICS;
    textRenderStage.clearColor = vec4(0,0,0,0);
    textRenderStage.renderPassFlags = HX_GRAPHICS_RENDERPASS_COLOUR;
    textRenderStage.clearFlags = textRenderStage.renderPassFlags;
    textRenderStage.outputTextures[0] = app.StrMapGet("fontColorUiRT");
    textRenderStage.pipeline = app.StrMapGet("fontRendererPipeline");
    HXShaderUniformMap& fnt_pip_map = app.renderer.registry.graphicsPipelines[HX_NULL_ID_GET_ID(textRenderStage.pipeline)].uniforms;
    textRenderStage.uniforms[0] = HXShaderInput{ app.hxg.GetUniform(fnt_pip_map, "GlyphsBuffer"), NULL };
    textRenderStage.uniforms[1] = HXShaderInput{ app.hxg.GetUniform(fnt_pip_map, "TextBuffer"), NULL };
    textRenderStage.uniforms[2] = HXShaderInput{ app.hxg.GetUniform(fnt_pip_map, "atlasTexture"), NULL, 1, &app.renderer.linearSampler };
    textRenderStage.uniforms[3] = HXShaderInput{ app.hxg.GetUniform(fnt_pip_map, "WindowConsts"), NULL };
    app.renderer.AddRenderStage(app, app.mainRenderChain, &textRenderStage, &RenderFonts);
}


FontID FontRenderer::loadFont(Application& app, const std::string& filename, uint encoding, HXCommandExecutionState cmd_state){
    Font font{};
    bool r = font.Init(app, filename, encoding, cmd_state);
    if (!r)return 0;
    return HX_NULL_ID_MAKE_ID(fonts.push_back(font));
}


void FontRenderer::RenderText(
    Application& app, const FontID& fontid, const vec2& position, const std::string& text, const float& scale
){
    if (!fontid)return;
    Font& font = fonts[HX_NULL_ID_GET_ID(fontid)];

    vec2 advance = vec2(0);
    for (size_t i=0; i<text.size(); ++i){
        unsigned char c = text[i]; // TODO: use encoding to combine chars to 32 bits
        if (c == '\n'){
            advance.x = 0.0f;
            advance.y -= 1.25f * scale;
            continue;
        }

        RenderTextChar gpu{};
        const vec4& offset = font.offsets[c];
        vec2 dimensions = vec2(offset.z, offset.w);
        vec2 pos = vec2(offset.x, offset.y);

        gpu.position = position * vec2(app.mainWindow.screenRatio, 1.0);
        gpu.position += advance;
        gpu.position = gpu.position + pos*scale;
        gpu.index = c;
        gpu.scale = dimensions*scale;
        memcpy(font.textBufferPtr + font.charCounter*sizeof(RenderTextChar), &gpu, sizeof(RenderTextChar));

        advance.x += font.advances[c] * scale;
        font.charCounter += 1;
    }
}


void FontRenderer::Update(Application& app){
    for (int i=0; i<fonts.size(); ++i){
        fonts[i].charCounter = 0;
    }
}


void FontRenderer::CleanUp(Application& app){

}
