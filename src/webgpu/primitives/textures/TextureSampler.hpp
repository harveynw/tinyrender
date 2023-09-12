#pragma once

namespace engine::Texture2D {
    class SamplerDelegate {
    public:
        virtual wgpu::Sampler createSampler(Context *context, unsigned int mips) = 0;

        virtual ~SamplerDelegate() = default;
    };

    class NoSampler : public SamplerDelegate {
    public:
        ~NoSampler() override = default;

        wgpu::Sampler createSampler(Context *context, unsigned int mips) override {
            (void) context;
            (void) mips;
            return nullptr;
        }
    };

    class DefaultSampler : public SamplerDelegate {
    public:
        ~DefaultSampler() override = default;

        wgpu::Sampler createSampler(Context *context, unsigned int mips) override {
            wgpu::SamplerDescriptor samplerDesc;
            samplerDesc.addressModeU = wgpu::AddressMode::MirrorRepeat; //AddressMode::ClampToEdge;
            samplerDesc.addressModeV = wgpu::AddressMode::MirrorRepeat;
            samplerDesc.addressModeW = wgpu::AddressMode::ClampToEdge;
            samplerDesc.magFilter = wgpu::FilterMode::Linear;
            samplerDesc.minFilter = wgpu::FilterMode::Linear;
            samplerDesc.mipmapFilter = wgpu::MipmapFilterMode::Linear;
            samplerDesc.lodMinClamp = 0.0f;
            samplerDesc.lodMaxClamp = (float) mips; // {1.0, 8.0}
            samplerDesc.compare = wgpu::CompareFunction::Undefined;
            samplerDesc.maxAnisotropy = 1;

            return context->device.createSampler(samplerDesc);
        }
    };
}