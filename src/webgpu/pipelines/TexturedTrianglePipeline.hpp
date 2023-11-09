//
// Created by Harvey Williams on 08/08/2023.
//

#ifndef TINYGAME_TEXTUREDTRIANGLEPIPELINE_H
#define TINYGAME_TEXTUREDTRIANGLEPIPELINE_H

#include <filesystem>
#include <iostream>

#include "Pipeline.hpp"
#include "../../objects/Object.hpp"
#include "tiny_obj_loader.h"

using glm::mat4x4;
using glm::vec4;
using glm::vec3;


class TexturedTrianglePipeline : public Pipeline {
protected:
    Context *context = nullptr;
    Scene *scene = nullptr;

    // Intermediate objects for setting up the pipeline, persisted here.
    DepthStencilState depthStencilState = Default;
    TextureFormat depthTextureFormat = TextureFormat::Undefined;
    wgpu::RenderPipeline pipeline = nullptr;
    wgpu::PipelineLayout layout = nullptr;

    // All necessary to be called pre onFrame(), can be done in any order.
    void initialiseShader(RenderPipelineDescriptor &desc);
    static void initialisePipelineOptions(RenderPipelineDescriptor &desc);
    void initialiseDepthStencil(RenderPipelineDescriptor &desc);

public:
    TexturedTrianglePipeline(Context *context, Scene *scene);
    ~TexturedTrianglePipeline() override;
    void onFrame(wgpu::TextureView &textureView, wgpu::CommandEncoder &commandEncoder,
                 std::vector<std::shared_ptr<tinyrender::Object>> &objects) override;
};

#endif
