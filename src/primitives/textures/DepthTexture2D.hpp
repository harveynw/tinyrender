//
// Created by Harvey Williams on 11/08/2023.
//

#ifndef TINYGAME_DEPTHTEXTURE2D_H
#define TINYGAME_DEPTHTEXTURE2D_H

#include "webgpu/webgpu.hpp"

#include "Texture2D.hpp"

class Engine;

namespace engine {

class DepthTexture2D : public engine::Texture2D {
    protected:
        wgpu::TextureFormat textureFormat = wgpu::TextureFormat::Depth24Plus;
    public:
        DepthTexture2D(Engine *engine, uint32_t width, uint32_t height);
        ~DepthTexture2D() = default;
    };

}


#endif //TINYGAME_DEPTHTEXTURE2D_H
