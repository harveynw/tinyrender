#pragma once

#include "../webgpu/primitives/buffers/uniforms/ModelMatrixUniform.hpp"

namespace engine {
    class Object {
        // Pipeline


        // WebGPU resources
        std::shared_ptr<engine::ModelMatrixUniform> modelMatrix; // this class should produce a bindgrouplayoutdesc for itself
        wgpu::BindGroup bindGroup;
    };
}