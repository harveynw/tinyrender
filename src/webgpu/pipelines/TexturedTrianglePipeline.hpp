#pragma once

#include <filesystem>
#include <iostream>

#include "Pipeline.hpp"
#include "objects/Object.hpp"
#include "tiny_obj_loader.h"

using glm::mat4x4;
using glm::vec4;
using glm::vec3;


class TexturedTrianglePipeline : public Pipeline {
protected:
    Context *context = nullptr;
    Scene *scene = nullptr;

    // Intermediate objects for setting up the pipeline, persisted here.
    wgpu::DepthStencilState depthStencilState = wgpu::Default;
    wgpu::TextureFormat depthTextureFormat = wgpu::TextureFormat::Undefined;
    wgpu::RenderPipeline pipeline = nullptr;
    wgpu::PipelineLayout layout = nullptr;

    // All necessary to be called pre onFrame(), can be done in any order.
    void initialiseShader(wgpu::RenderPipelineDescriptor &desc);
    static void initialisePipelineOptions(wgpu::RenderPipelineDescriptor &desc);
    void initialiseDepthStencil(wgpu::RenderPipelineDescriptor &desc);

public:
    TexturedTrianglePipeline(Context *context, Scene *scene);
    ~TexturedTrianglePipeline() override;
    void onFrame(wgpu::TextureView &textureView, wgpu::CommandEncoder &commandEncoder,
                std::vector<ObjectImpl*>&) override;
};
