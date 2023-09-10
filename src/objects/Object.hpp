#pragma once

#include "../webgpu/primitives/buffers/uniforms/UniformModel.hpp"

namespace engine {
    class Object {
        // Pipeline


        // WebGPU resources
        std::shared_ptr<engine::UniformModel> modelMatrix; // this class should produce a bindgrouplayoutdesc for itself
        wgpu::BindGroup bindGroup;
    };
}