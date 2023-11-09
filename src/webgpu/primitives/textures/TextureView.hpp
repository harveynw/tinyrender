#pragma once


namespace tinyrender::Texture2D {
    class ViewDelegate {
    public:
        virtual wgpu::TextureView createView(wgpu::Texture texture, unsigned int mips) = 0;
        virtual ~ViewDelegate() = default;
    };

    class NoView : public ViewDelegate {
    public:
        ~NoView() override = default;

        wgpu::TextureView createView(wgpu::Texture texture, unsigned int mips) override {
            (void) texture;
            (void) mips;
            return nullptr;
        }
    };

    class RGBAView : public ViewDelegate {
    public:
        ~RGBAView() override = default;

        wgpu::TextureView createView(wgpu::Texture texture, unsigned int mips) override {
            wgpu::TextureViewDescriptor textureViewDesc;
            textureViewDesc.aspect = wgpu::TextureAspect::All;
            textureViewDesc.baseArrayLayer = 0;
            textureViewDesc.arrayLayerCount = 1;
            textureViewDesc.baseMipLevel = 0;
            textureViewDesc.mipLevelCount = mips;
            textureViewDesc.dimension = wgpu::TextureViewDimension::_2D;
            textureViewDesc.format = wgpu::TextureFormat::RGBA8Unorm;

            auto view = texture.createView(textureViewDesc);

            std::cout << "Texture view: " << view << std::endl;
            return view;
        }
    };

    class DepthView : public ViewDelegate {
    public:
        ~DepthView() override = default;

        wgpu::TextureView createView(wgpu::Texture texture, unsigned int mips) override {
            (void) mips;

            wgpu::TextureViewDescriptor textureViewDesc;
            textureViewDesc.aspect = wgpu::TextureAspect::DepthOnly;
            textureViewDesc.baseArrayLayer = 0;
            textureViewDesc.arrayLayerCount = 1;
            textureViewDesc.baseMipLevel = 0;
            textureViewDesc.mipLevelCount = 1;
            textureViewDesc.dimension = wgpu::TextureViewDimension::_2D;
            textureViewDesc.format = wgpu::TextureFormat::Depth24Plus;

            auto view = texture.createView(textureViewDesc);

            std::cout << "Texture view: " << view << std::endl;
            return view;
        }
    };
}