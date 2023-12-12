#pragma once

#include <webgpu/webgpu.hpp>

class Uniform {
public:
    virtual wgpu::BindGroupEntry generateUniformBindGroupEntry(int bindGroup) = 0;
    virtual ~Uniform() = default;
};