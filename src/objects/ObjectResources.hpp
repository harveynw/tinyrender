#pragma once

#include <glm/glm.hpp>
#include <utility>
#include <webgpu/webgpu.hpp>

#include "../webgpu/primitives/buffers/AttributeBuffer.hpp"
#include "../webgpu/primitives/buffers/IndexBuffer.hpp"
#include "../webgpu/primitives/buffers/uniforms/ModelMatrixUniform.hpp"
#include "../webgpu/primitives/buffers/uniforms/ColorUniform.hpp"
#include "../webgpu/primitives/textures/Texture2D.hpp"
#include "../webgpu/Scene.hpp"


enum ObjectPipeline {
    TexturedTriangle,
    ColoredTriangle
};

// All the WebGPU resources owned by the object
struct ObjectResources {
    bool isIndexed = false;

    std::shared_ptr<engine::AttributeBuffer> attributeBuffer = nullptr;
    std::shared_ptr<engine::IndexBuffer> indexBuffer = nullptr;
    std::shared_ptr<engine::Texture2D::Texture> texture = nullptr;
    std::shared_ptr<engine::ModelMatrixUniform> modelMatrix = nullptr;
    std::shared_ptr<engine::ColorUniform> color = nullptr;

    std::vector<wgpu::BindGroupEntry> bindGroupData;
    wgpu::BindGroup bindGroup = nullptr;

    ObjectResources(Context *context, Scene *scene, std::shared_ptr<engine::AttributeBuffer> attrs, ObjectPipeline target);
    ~ObjectResources();

    void resetBindGroup(Context *context, Scene *scene, ObjectPipeline target);
};