#ifndef TINYGAME_BUFFER_H
#define TINYGAME_BUFFER_H

#include "webgpu/webgpu.hpp"
#include "../../Context.hpp"

/*
 * Buffer
 *
 * An abstraction on top of WebGPU for storing data in GPU memory.
 * Subclasses of this can be used for managing uniforms, vertex data
 * and more with specialised methods for updating them.
 */

using namespace wgpu;

namespace tinyrender {

    enum BufferType {
        VERTEX = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Vertex,
        UNIFORM = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Uniform,
        INDEX = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Index
    };

    class Buffer {
    protected:
        Context *context = nullptr;

        wgpu::Buffer underlying = nullptr; // wgpu buffer

        BufferType type;
        uint64_t size;
        bool mapped;
    public:
        Buffer() {};
        Buffer(Context *context, BufferType type, uint64_t size, bool mapped, void* data);
        ~Buffer();

        uint64_t getSize() { return size; };
        void initialise(void*);
    };

}


#endif //TINYGAME_BUFFER_H
