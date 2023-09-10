//
// Created by Harvey Williams on 09/08/2023.
//

#pragma once

#include <utility>

#include "webgpu/webgpu.hpp"
#include "glm/glm.hpp"

#include "../../../engine.hpp"
#include "TextureCreator.hpp"
#include "TextureData.hpp"
#include "TextureView.hpp"
#include "TextureSampler.hpp"

/*
 * Texture2D
 *
 * An abstraction on top of WebGPU Textures that are 2-dimensional. Texture2D creation is compositional,
 * although not all combinations of delegates may necessarily work; so study of the underlying WebGPU code is still
 * needed. Instances of this can handle common texture constructions from formulas and files as well as other useful
 * resources such as depth textures.
 */

class Engine;


namespace engine::Texture2D {

    class Texture {
    protected:
        Engine *engine = nullptr;

        unsigned int width = 0;
        unsigned int height = 0;
        unsigned int mips = 1;

        // Delegates for texture
        engine::Texture2D::DataDelegate *_data;
        engine::Texture2D::CreationDelegate *_create;
        engine::Texture2D::ViewDelegate *_view;
        engine::Texture2D::SamplerDelegate *_samp;

    public:
        // Resources
        wgpu::Texture underlying = nullptr;
        wgpu::TextureView textureView = nullptr;
        wgpu::Sampler sampler = nullptr;

        Texture(Engine *engine, engine::Texture2D::DataDelegate *data, engine::Texture2D::CreationDelegate *create,
                  engine::Texture2D::ViewDelegate *view, engine::Texture2D::SamplerDelegate *samp);
        ~Texture();

        wgpu::TextureView getView() {
            if(textureView == nullptr)
                throw std::runtime_error("Texture view requested but was not created for this instance");
            return textureView;
        };

        wgpu::Sampler getSampler() {
            if(sampler == nullptr)
                throw std::runtime_error("Sampler requested but none associated with texture");
            return sampler;
        }
    };

    namespace common {

        class BasicImgTexture : public Texture {
        public:
            BasicImgTexture(Engine *engine, std::string path) : Texture(engine,
                                                                      new ImgData(std::move(path)),
                                                                      new RGBA(),
                                                                      new RGBAView(),
                                                                      new DefaultSampler()) {}
        };

        class DebugTexture : public Texture {
        public:
            DebugTexture(Engine *engine, unsigned int width, unsigned int height) : Texture(engine,
                                                                      new DebugData(width, height),
                                                                      new RGBA(),
                                                                      new RGBAView(),
                                                                      new DefaultSampler()) {}
        };

        class DefaultDepthTexture : public Texture {
        public:
            explicit DefaultDepthTexture(Engine *engine) : Texture(engine,
                                                                   new NoData(),
                                                                   new Depth2D(),
                                                                   new DepthView(),
                                                                   new NoSampler()) {}
        };
    }
}
