#pragma once

#include <glm/glm.hpp>
#include <utility>
#include <webgpu/webgpu.hpp>

#include "../webgpu/primitives/buffers/AttributeBuffer.hpp"
#include "../webgpu/primitives/buffers/IndexBuffer.hpp"
#include "../webgpu/primitives/uniforms/ModelMatrixUniform.hpp"
#include "../webgpu/primitives/uniforms/ColorUniform.hpp"
#include "../webgpu/primitives/uniforms/ScalarUniform.hpp"
#include "../webgpu/primitives/uniforms/TextureUniforms.hpp"
#include "../webgpu/primitives/textures/Texture2D.hpp"
#include "../webgpu/primitives/textures/CharColorTexture.hpp"
#include "../webgpu/Scene.hpp"
#include "../webgpu/pipelines/ObjectPipeline.hpp"


// All the WebGPU resources owned by the object
struct ObjectResources {
    Context *context;
    Scene *scene;

    ObjectPipeline targetPipeline;

    bool isIndexed = false;

    std::shared_ptr<AttributeBuffer> attributeBuffer = nullptr;
    std::shared_ptr<IndexBuffer> indexBuffer = nullptr;
    std::shared_ptr<Texture2D::Texture> texture = nullptr;
    std::shared_ptr<ModelMatrixUniform> modelMatrix = nullptr;
    std::shared_ptr<ColorUniform> color = nullptr;

    // WaveSim specific
    std::shared_ptr<ScalarUniform> maxDisplacement = nullptr;

    // Voxel specific
    std::shared_ptr<ModelMatrixUniform> globalModelMatrix = nullptr;
    std::shared_ptr<Texture2D::CharColorTexture> voxelColors = nullptr;

    ObjectResources(Context *context, Scene *scene, std::shared_ptr<AttributeBuffer> attrs, ObjectPipeline target);
    ~ObjectResources();

    std::vector<wgpu::BindGroupEntry> bindGroupData;
    wgpu::BindGroup bindGroup = nullptr;
    void resetBindGroup(ObjectPipeline);
};
