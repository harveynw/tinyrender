//
// Created by Harvey Williams on 09/08/2023.
//

#ifndef TINYGAME_TEXTURE_H
#define TINYGAME_TEXTURE_H

#include "webgpu/webgpu.hpp"
#include "glm/glm.hpp"
#include "../../engine.hpp"

/*
 * Texture2D
 *
 * An abstraction on top of WebGPU Textures that are 2-dimensional. The base class is not particularly useful
 * besides explicitly handling destruction of the underlying texture. Subclasses of this can handle common texture
 * constructions from formulas and files as well as other useful resources such as depth textures.
 */

class Engine;

namespace engine {

    class Texture2D {
    protected:
        Engine *engine = nullptr;

        wgpu::Texture underlying = nullptr;
        wgpu::TextureView view = nullptr;

        uint64_t size = 0;
        wgpu::TextureDescriptor textureDesc;
        wgpu::ImageCopyTexture destination;
        wgpu::TextureDataLayout source;
    public:
        Texture2D() = default;
        Texture2D(Engine *engine, wgpu::TextureDescriptor textureDesc,
                  wgpu::ImageCopyTexture destination, wgpu::TextureDataLayout source, uint64_t size, void *data);
        ~Texture2D();
        wgpu::TextureView getView() { return view; };
        void initialise(void *data);
    };

}


#endif //TINYGAME_TEXTURE_H
