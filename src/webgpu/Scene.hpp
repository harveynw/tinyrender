#pragma once

#include "webgpu/webgpu.hpp"
#include "primitives/textures/Texture2D.hpp"
#include "primitives/uniforms/LightingUniform.hpp"
#include "primitives/shaders/TexturedShader.hpp"
#include "primitives/shaders/ColoredShader.hpp"
#include "primitives/shaders/WavesShader.hpp"
#include "primitives/shaders/VoxelShader.hpp"
#include "primitives/uniforms/ViewProjMatrixUniform.hpp"
#include "pipelines/ObjectPipeline.hpp"

/*
 * Scene
 * Encapsulates any abstracted WebGPU resources that many classes require access to.
 */

struct Scene {
    // Depth Buffer, pipelines will require read/write to this.
    std::shared_ptr<Texture2D::Texture> depthTexture;

    // Shaders
    std::map<ObjectPipeline, std::shared_ptr<Shader>> shaders = {
        { TexturedTriangle, nullptr },
        { ColoredTriangle , nullptr },
        { Waves, nullptr },
        { Voxels, nullptr}
    };

    // ViewProjection + Lighting data
    std::shared_ptr<LightingUniform> lightingUniform;
    std::shared_ptr<ViewProjMatrixUniform> viewProjUniform;
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
