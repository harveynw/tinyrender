#pragma once

#include <glm/glm.hpp>
#include <utility>
#include <webgpu/webgpu.hpp>

#include "Fwd.hpp"
#include "../webgpu/primitives/buffers/AttributeBuffer.hpp"
#include "../webgpu/primitives/buffers/IndexBuffer.hpp"
#include "../webgpu/primitives/uniforms/ModelMatrixUniform.hpp"
#include "../webgpu/primitives/uniforms/ColorUniform.hpp"
#include "../webgpu/primitives/uniforms/ScalarUniform.hpp"
#include "../webgpu/primitives/uniforms/TextureUniforms.hpp"
#include "../webgpu/primitives/textures/Texture2D.hpp"
#include "../webgpu/Scene.hpp"

using tinyrender::TextureUniform;
using tinyrender::TextureSamplerUniform;

// All the WebGPU resources owned by the object
struct ObjectResources {
    Context *context;
    Scene *scene;

    ObjectPipeline targetPipeline;

    bool isIndexed = false;

    std::shared_ptr<tinyrender::AttributeBuffer> attributeBuffer = nullptr;
    std::shared_ptr<tinyrender::IndexBuffer> indexBuffer = nullptr;
    std::shared_ptr<tinyrender::Texture2D::Texture> texture = nullptr;
    std::shared_ptr<tinyrender::ModelMatrixUniform> modelMatrix = nullptr;
    std::shared_ptr<tinyrender::ColorUniform> color = nullptr;

    // WaveSim specific
    std::shared_ptr<tinyrender::ScalarUniform> maxDisplacement = nullptr;

    // Voxel specific
    std::shared_ptr<tinyrender::ModelMatrixUniform> globalModelMatrix = nullptr;

    ObjectResources(Context *context, Scene *scene, std::shared_ptr<tinyrender::AttributeBuffer> attrs, ObjectPipeline target);
    ~ObjectResources();

    std::vector<wgpu::BindGroupEntry> bindGroupData;
    wgpu::BindGroup bindGroup = nullptr;
    void resetBindGroup(ObjectPipeline);
};
