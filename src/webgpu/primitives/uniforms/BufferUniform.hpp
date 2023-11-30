#pragma once

#include "Uniform.hpp"
#include "../buffers/Buffer.hpp"


namespace tinyrender {

    class BufferUniform : public Uniform, public tinyrender::Buffer {
    public:
        wgpu::BindGroupEntry generateUniformBindGroupEntry(int bindGroup) override {
            BindGroupEntry entry;
            entry.binding = bindGroup;
            entry.buffer = underlying;
            entry.offset = 0;
            entry.size = getSize();

            return entry;
        };
    };

}