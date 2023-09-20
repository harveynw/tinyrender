#pragma once

#include "../webgpu/primitives/buffers/uniforms/ModelMatrixUniform.hpp"
#include "../webgpu/primitives/buffers/attributes/AttributeBuffer.hpp"

namespace engine {

    // TODO: Delete IndexedTrianglePipeline and create isIndexed: bool and indexBuffer (empty by default), pipeline can use that

    enum ObjectPipeline {
        Indexed,
        TexturedTriangle,
        Triangle
    };

    class Object {
        ObjectPipeline destinationPipeline;

        // WebGPU resources
        std::shared_ptr<engine::AttributeBuffer> data = nullptr;
        std::shared_ptr<engine::ModelMatrixUniform> modelMatrix = nullptr; // this class should produce a bindgrouplayoutdesc for itself
        wgpu::BindGroup bindGroup = nullptr;

        Object(Context *c, DataDelegate *data): _data(data) {


        };
    };
}