//
// Created by Harvey Williams on 09/08/2023.
//

#ifndef TINYGAME_BUFFER_H
#define TINYGAME_BUFFER_H

#include "webgpu/webgpu.hpp"
#include "../../../engine.hpp"

/*
 * Buffer
 *
 * An abstraction on top of WebGPU for storing data in GPU memory.
 * Subclasses of this can be used for managing uniforms, vertex data
 * and more with specialised methods for updating them.
 */

namespace engine {

    enum BufferType {
        VERTEX = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Vertex,
        UNIFORM = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Uniform,
        INDEX = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Index
    };

    class Buffer {
    protected:
        Engine *engine = nullptr; // Engine the buffer is associated with

        wgpu::Buffer underlying = nullptr; // wgpu buffer

        BufferType type;
        uint64_t size;
        bool mapped;
    public:
        Buffer() {};
        Buffer(Engine *engine, BufferType type, uint64_t size, bool mapped, void* data);
        ~Buffer();

        uint64_t getSize() { return size; };
        void initialise(void*);

        BindGroupEntry generateUniformBindGroupEntry(int bindGroup);
    };

}


#endif //TINYGAME_BUFFER_H
