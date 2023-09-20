#pragma once

#include "webgpu/webgpu.hpp"
#include "primitives/textures/Texture2D.hpp"
#include "primitives/buffers/uniforms/LightingUniform.hpp"

/*
 * Scene
 * Encapsulates any abstracted WebGPU resources that many classes require access to.
 */

struct Scene {
    // Depth Buffer, pipelines will require read/write to this.
    std::shared_ptr<engine::Texture2D::Texture> depthTexture;

    void buildDepthBuffer(const std::shared_ptr<Context>& context) {
        depthTexture = std::make_shared<engine::Texture2D::common::DefaultDepthTexture>(context.get());
    }

    // Lighting data
    std::shared_ptr<engine::LightingUniform> lightingUniform;
};
