#pragma once

#include <webgpu/webgpu.hpp>


namespace tinyrender {
    class Uniform {
    public:
        virtual wgpu::BindGroupEntry generateUniformBindGroupEntry(int bindGroup) = 0;
        virtual ~Uniform() = default;
    };
}