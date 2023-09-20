//
// Created by Harvey Williams on 09/08/2023.
//

#pragma once

#include <utility>

#include "webgpu/webgpu.hpp"
#include "glm/glm.hpp"

#include "../../Context.hpp"
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


namespace engine::Texture2D {

    class Texture {
    protected:
        Context *context = nullptr;

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

        Texture(Context *context, engine::Texture2D::DataDelegate *data, engine::Texture2D::CreationDelegate *create,
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
            BasicImgTexture(Context *context, std::string path) : Texture(context,
                                                                      new ImgData(std::move(path)),
                                                                      new RGBA(),
                                                                      new RGBAView(),
                                                                      new DefaultSampler(false, true, false)) {}
        };

        class BasicImgRepeatingTexture : public Texture {
        public:
            BasicImgRepeatingTexture(Context *context, std::string path) : Texture(context,
                                                                          new ImgData(std::move(path)),
                                                                          new RGBA(),
                                                                          new RGBAView(),
                                                                          new DefaultSampler()) {}
        };

        class DebugTexture : public Texture {
        public:
            DebugTexture(Context *context, unsigned int width, unsigned int height) : Texture(context,
                                                                      new DebugData(width, height, false),
                                                                      new RGBA(),
                                                                      new RGBAView(),
                                                                      new DefaultSampler(false, false, true)) {}
        };

        class DefaultDepthTexture : public Texture {
        public:
            explicit DefaultDepthTexture(Context *context) : Texture(context,
                                                                   new NoData(),
                                                                   new Depth2D(),
                                                                   new DepthView(),
                                                                   new NoSampler()) {}
        };
    }
}
