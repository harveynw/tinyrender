#pragma once

#include "Buffer.hpp"

namespace engine {

    class AttributeBuffer : public engine::Buffer {
    protected:
        int nDrawCalls; // Typically the number of vertex shader invocations, i.e. the expected number of draw calls
    public:
        AttributeBuffer(Context *context, std::vector<float> &data, int nDrawCalls);
        int getNDrawCalls() { return nDrawCalls; };
        wgpu::Buffer getUnderlyingBuffer() { return this->underlying; };
    };

}