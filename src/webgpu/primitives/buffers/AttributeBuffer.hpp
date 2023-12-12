#pragma once

#include "Buffer.hpp"


class AttributeBuffer : public tr::Buffer {
protected:
    int nDrawCalls; // Typically the number of vertex shader invocations, i.e. the expected number of draw calls
    size_t initialSize;

    std::vector<float> cpuBuffer; // TODO: AttributeBuffer keeps the data on the CPU for now, need to think of a method to drop it when possible (after queue write to GPU)
public:
    AttributeBuffer(Context *context, std::vector<float> &data, int nDrawCalls);

    int getDrawCalls() { return nDrawCalls; };

    void update(std::vector<float> &newData, int nDrawCalls);

    wgpu::Buffer getUnderlyingBuffer() { return this->underlying; };
};
