//
// Created by Harvey Williams on 08/08/2023.
//

#pragma once

#include "Pipeline.hpp"
#include "../../engine.hpp"
#include "../primitives/textures/Texture2D.hpp"
#include "../primitives/buffers/uniforms/UniformViewProjection.hpp"
#include "../primitives/buffers/uniforms/UniformModel.hpp"
#include "../primitives/buffers/IndexBuffer.hpp"
#include "../primitives/buffers/attributes/IndexedAttribute.hpp"

using namespace wgpu;
using glm::mat4x4;
using glm::vec4;
using glm::vec3;

// Pair a model matrix with data to be fed into the vertex shader
struct IndexedTriangleObject {
    std::shared_ptr<engine::UniformModel> modelMatrix;
    std::shared_ptr<engine::IndexBuffer> indexData;
    std::shared_ptr<engine::IndexedAttribute> vertexData;
};

/**
    * A structure that describes the data layout in the vertex buffer
    * We do not instantiate it but use it in `sizeof` and `offsetof`
    */
struct IndexedTriangleVertexAttributes {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
};

class IndexedTrianglePipeline : public Pipeline {
protected:
    Context *context = nullptr;
    Scene *scene = nullptr;

    std::shared_ptr<engine::UniformViewProjection> uniforms;

    wgpu::RenderPipeline pipeline = nullptr;

    std::vector<IndexedTriangleObject> objects;
    std::vector<std::vector<BindGroupEntry>> bindGroupEntries;
    std::vector<BindGroup> bindGroups;

    // Intermediate objects for setting up the pipeline, persisted here.
    wgpu::ShaderModule shaderModule = nullptr;
    wgpu::FragmentState fragmentState;
    wgpu::BlendState blendState;
    wgpu::ColorTargetState colorTarget;
    DepthStencilState depthStencilState = Default;
    TextureFormat depthTextureFormat = TextureFormat::Undefined;
    std::vector<VertexAttribute> vertexAttribs;
    VertexBufferLayout vertexBufferLayout;
    std::vector<BindGroupLayoutEntry> bindingLayoutEntries;
    BindGroupLayout bindGroupLayout = nullptr;
    PipelineLayout layout = nullptr;

    // All necessary to be called pre onFrame(), can be done in any order.
    void initialiseBuffers();
    void initialiseShader(RenderPipelineDescriptor &desc);
    static void initialisePipelineOptions(RenderPipelineDescriptor &desc);
    void initialiseDepthBufferAndStencil(RenderPipelineDescriptor &desc);
    void initialiseAttributes(RenderPipelineDescriptor &desc);
    void initialiseUniformBindGroup(RenderPipelineDescriptor &desc);

public:
    IndexedTrianglePipeline(Context *context, Scene *scene, std::shared_ptr<engine::UniformViewProjection> uniforms, std::vector<IndexedTriangleObject> &objects);
    ~IndexedTrianglePipeline() override;
    void onFrame(wgpu::TextureView &textureView, wgpu::CommandEncoder &commandEncoder) override;
};
