#pragma once

#include <webgpu/webgpu.hpp>
#include "../../../engine.hpp"

namespace engine::Texture2D {

    class CreationDelegate {
    public:
        virtual wgpu::Texture createTexture(Engine *engine, unsigned int width, unsigned int height,
                                            unsigned int mips) = 0;
        virtual ~CreationDelegate() = default;
    };

    class RGBA : public CreationDelegate {
    public:
        RGBA() = default;
        ~RGBA() override = default;

        wgpu::Texture createTexture(Engine *engine, unsigned int width, unsigned int height, unsigned int mips) override {
            wgpu::TextureDescriptor textureDesc;

            textureDesc.dimension = TextureDimension::_2D;
            textureDesc.size = { width, height, 1 };
            textureDesc.mipLevelCount = mips;
            textureDesc.sampleCount = 1;
            textureDesc.format = TextureFormat::RGBA8Unorm;
            textureDesc.usage = TextureUsage::TextureBinding | TextureUsage::CopyDst;
            textureDesc.viewFormatCount = 0;
            textureDesc.viewFormats = nullptr;

            auto texture = engine->wgpuGetDevice().createTexture(textureDesc);
            std::cout << "Texture: " << texture << std::endl;
            return texture;
        }
    };

    class Depth2D : public CreationDelegate {
    protected:
        wgpu::TextureFormat textureFormat = wgpu::TextureFormat::Depth24Plus;
    public:
        Depth2D() = default;
        ~Depth2D() override = default;

        wgpu::Texture createTexture(Engine *engine, unsigned int width, unsigned int height, unsigned int mips) override {
            (void) width;
            (void) height;
            (void) mips;

            wgpu::TextureDescriptor textureDesc;

            textureDesc.dimension = TextureDimension::_2D;
            textureDesc.format = textureFormat;
            textureDesc.mipLevelCount = 1;
            textureDesc.sampleCount = 1;
            textureDesc.size = {(unsigned int) engine->DISPLAY_WIDTH,
                                (unsigned int) engine->DISPLAY_HEIGHT, 1};
            textureDesc.usage = TextureUsage::RenderAttachment;
            textureDesc.viewFormatCount = 1;
            textureDesc.viewFormats = (WGPUTextureFormat*)&textureFormat;

            auto texture = engine->wgpuGetDevice().createTexture(textureDesc);
            std::cout << "Depth texture: " << texture << std::endl;
            return texture;
        }
    };

}