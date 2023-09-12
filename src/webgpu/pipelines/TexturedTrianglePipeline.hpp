//
// Created by Harvey Williams on 08/08/2023.
//

#ifndef TINYGAME_TEXTUREDTRIANGLEPIPELINE_H
#define TINYGAME_TEXTUREDTRIANGLEPIPELINE_H

#include <filesystem>
#include <iostream>

#include "Pipeline.hpp"
#include "../../engine.hpp"
#include "../primitives/textures/Texture2D.hpp"
#include "../primitives/buffers/uniforms/ViewProjMatrixUniform.hpp"
#include "../primitives/buffers/uniforms/ModelMatrixUniform.hpp"
#include "../primitives/buffers/attributes/TexturedAttribute.hpp"

using namespace wgpu;
using glm::mat4x4;
using glm::vec4;
using glm::vec3;


// Pair a model matrix with data to be fed into the vertex shader
struct TexturedTriangleObject {
    std::shared_ptr<engine::ModelMatrixUniform> modelMatrix;
    std::shared_ptr<engine::TexturedAttribute> vertexData;
};


class TexturedTrianglePipeline : public Pipeline {
protected:
    Context *context = nullptr;
    Scene *scene = nullptr;

    std::shared_ptr<engine::ViewProjMatrixUniform> uniforms;
    std::shared_ptr<engine::Texture2D::Texture> texture;

    wgpu::RenderPipeline pipeline = nullptr;

    std::vector<TexturedTriangleObject> objects;
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
    void initialiseBuffers() {};
    void initialiseShader(RenderPipelineDescriptor &desc);
    static void initialisePipelineOptions(RenderPipelineDescriptor &desc);
    void initialiseDepthBufferAndStencil(RenderPipelineDescriptor &desc);
    void initialiseAttributes(RenderPipelineDescriptor &desc);
    void initialiseUniformBindGroup(RenderPipelineDescriptor &desc);

public:
    TexturedTrianglePipeline(Context *context,
                             Scene *scene,
                             std::shared_ptr<engine::ViewProjMatrixUniform> uniforms,
                             std::shared_ptr<engine::Texture2D::Texture> texture,
                             std::vector<TexturedTriangleObject> &objects);
    ~TexturedTrianglePipeline() override;
    void onFrame(wgpu::TextureView &textureView, wgpu::CommandEncoder &commandEncoder) override;
};

bool loadTexturedObjIntoTriangleData(std::string path, std::vector<UVTriangleVertexAttributes> &vertexData);


#endif
