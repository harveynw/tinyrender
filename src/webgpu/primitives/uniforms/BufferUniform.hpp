#pragma once

#include "Uniform.hpp"
#include "../buffers/Buffer.hpp"



class BufferUniform : public Uniform, public tr::Buffer {
public:
    wgpu::BindGroupEntry generateUniformBindGroupEntry(int bindGroup) override {
        wgpu::BindGroupEntry entry;
        entry.binding = bindGroup;
        entry.buffer = underlying;
        entry.offset = 0;
        entry.size = getSize();

        return entry;
    };
};
