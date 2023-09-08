#pragma once

#include "../../../engine.hpp"

namespace engine::Texture2D {
    class ViewDelegate {
    public:
        virtual wgpu::TextureView createView(wgpu::Texture texture) = 0;
        virtual ~ViewDelegate() = default;
    };

    class NoView : public ViewDelegate {
    public:
        ~NoView() override = default;

        wgpu::TextureView createView(wgpu::Texture texture) override {
            (void) texture;
            return nullptr;
        }
    };

    class RGBANoMips : public ViewDelegate {
    public:
        ~RGBANoMips() override = default;

        wgpu::TextureView createView(wgpu::Texture texture) override {
            TextureViewDescriptor textureViewDesc;
            textureViewDesc.aspect = TextureAspect::All;
            textureViewDesc.baseArrayLayer = 0;
            textureViewDesc.arrayLayerCount = 1;
            textureViewDesc.baseMipLevel = 0;
            textureViewDesc.mipLevelCount = 1;
            textureViewDesc.dimension = TextureViewDimension::_2D;
            textureViewDesc.format = TextureFormat::RGBA8Unorm;

            auto view = texture.createView(textureViewDesc);

            std::cout << "Texture view: " << view << std::endl;
            return view;
        }
    };

    class DepthView : public ViewDelegate {
    public:
        ~DepthView() override = default;

        wgpu::TextureView createView(wgpu::Texture texture) override {
            TextureViewDescriptor textureViewDesc;
            textureViewDesc.aspect = TextureAspect::DepthOnly;
            textureViewDesc.baseArrayLayer = 0;
            textureViewDesc.arrayLayerCount = 1;
            textureViewDesc.baseMipLevel = 0;
            textureViewDesc.mipLevelCount = 1;
            textureViewDesc.dimension = TextureViewDimension::_2D;
            textureViewDesc.format = wgpu::TextureFormat::Depth24Plus;

            auto view = texture.createView(textureViewDesc);

            std::cout << "Texture view: " << view << std::endl;
            return view;
        }
    };
}