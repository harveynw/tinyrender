//
// Created by Harvey Williams on 08/08/2023.
//

#ifndef TINYGAME_TEXTUREDTRIANGLEPIPELINE_H
#define TINYGAME_TEXTUREDTRIANGLEPIPELINE_H

#include <filesystem>
#include <iostream>

#include "Pipeline.hpp"
#include "../engine.hpp"
#include "../primitives/buffers/uniforms/UniformViewProjection.hpp"
#include "../primitives/textures/DepthTexture2D.hpp"
#include "../primitives/buffers/uniforms/UniformModel.hpp"
#include "../primitives/buffers/attributes/TexturedAttribute.hpp"

using glm::mat4x4;
using glm::vec4;
using glm::vec3;


// Pair a model matrix with data to be fed into the vertex shader
struct TexturedTriangleObject {
    std::shared_ptr<engine::UniformModel> modelMatrix;
    std::shared_ptr<engine::TexturedAttribute> vertexData;
};


class TexturedTrianglePipeline : public Pipeline {
protected:
    Engine *engine = nullptr;
    std::shared_ptr<engine::UniformViewProjection> uniforms;
    std::shared_ptr<engine::DepthTexture2D> depthTexture;
    std::shared_ptr<engine::Texture2D> texture;

    wgpu::RenderPipeline pipeline = nullptr;

    std::vector<TexturedTriangleObject> objects;
    std::vector<std::vector<BindGroupEntry>> bindGroupEntries;
    std::vector<BindGroup> bindGroups;


    // Intermediate objects for setting up the pipeline, persisted here.
    wgpu::Sampler sampler = nullptr;
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
    void initialiseBuffers() {};
    void initialiseShader(RenderPipelineDescriptor &desc);
    static void initialisePipelineOptions(RenderPipelineDescriptor &desc);
    void initialiseDepthBufferAndStencil(RenderPipelineDescriptor &desc);
    void initialiseAttributes(RenderPipelineDescriptor &desc);
    void initialiseUniformBindGroup(RenderPipelineDescriptor &desc);

public:
    TexturedTrianglePipeline(Engine *engine, std::shared_ptr<engine::UniformViewProjection> uniforms, std::shared_ptr<engine::Texture2D> texture, std::shared_ptr<engine::DepthTexture2D> depthTexture,
                             std::vector<TexturedTriangleObject> &objects);
    ~TexturedTrianglePipeline() override;
    void onFrame(wgpu::TextureView &textureView, wgpu::CommandEncoder &commandEncoder) override;
};

namespace fs = std::__fs::filesystem;
bool loadTexturedObjIntoTriangleData(const fs::path& path, std::vector<UVTriangleVertexAttributes> &vertexData);


#endif
