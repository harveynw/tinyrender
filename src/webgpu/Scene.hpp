#pragma once

#include "webgpu/webgpu.hpp"
#include "tinyrender/Fwd.hpp"
#include "primitives/textures/Texture2D.hpp"
#include "primitives/uniforms/LightingUniform.hpp"
#include "primitives/shaders/TexturedShader.hpp"
#include "primitives/shaders/ColoredShader.hpp"
#include "primitives/shaders/WavesShader.hpp"
#include "primitives/shaders/VoxelShader.hpp"
#include "primitives/uniforms/ViewProjMatrixUniform.hpp"

/*
 * Scene
 * Encapsulates any abstracted WebGPU resources that many classes require access to.
 */

struct Scene {
    // Depth Buffer, pipelines will require read/write to this.
    std::shared_ptr<tinyrender::Texture2D::Texture> depthTexture;

    // Shaders
    std::map<ObjectPipeline, std::shared_ptr<tinyrender::Shader>> shaders = {
        { TexturedTriangle, nullptr },
        { ColoredTriangle , nullptr },
        { Waves, nullptr },
        { Voxels, nullptr}
    };

    // ViewProjection + Lighting data
    std::shared_ptr<tinyrender::LightingUniform> lightingUniform;
    std::shared_ptr<tinyrender::ViewProjMatrixUniform> viewProjUniform;
    std::vector<wgpu::BindGroupEntry> viewProjUniformBindGroupData;
    std::map<ObjectPipeline, wgpu::BindGroup> bindGroups = {
        { TexturedTriangle, nullptr },
        { ColoredTriangle, nullptr },
        { Waves, nullptr },
        { Voxels, nullptr }
    };

    Scene(std::shared_ptr<Context> context);

    void buildDepthBuffer(const std::shared_ptr<Context>& context);

    ~Scene() {
        for(auto bg : bindGroups)
            bg.second.release();
    }
};
