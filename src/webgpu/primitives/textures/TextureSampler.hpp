#pragma once

namespace Texture2D {
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
    protected:
        WGPUAddressMode _addressMode = wgpu::AddressMode::Repeat;
        WGPUFilterMode _filterMode = wgpu::FilterMode::Nearest;
        WGPUMipmapFilterMode _mipsFilterMode = wgpu::MipmapFilterMode::Linear;

    public:
        DefaultSampler() = default;
        DefaultSampler(bool clamp, bool linearResize, bool nearestMips) {
            if(clamp)
                _addressMode = wgpu::AddressMode::ClampToEdge;
            if(linearResize)
                _filterMode = wgpu::FilterMode::Linear;
            if(nearestMips)
                _mipsFilterMode = wgpu::MipmapFilterMode::Nearest;
        };

        ~DefaultSampler() override = default;

        wgpu::Sampler createSampler(Context *context, unsigned int mips) override {
            wgpu::SamplerDescriptor samplerDesc;

            // How the sampler should map the texture to UV coordinates
            samplerDesc.addressModeU = _addressMode;
            samplerDesc.addressModeV = _addressMode;
            samplerDesc.addressModeW = _addressMode;

            // How the sampler should filter pixels when sampling smaller/larger than one pixel
            samplerDesc.magFilter = _filterMode;
            samplerDesc.minFilter = _filterMode;

            // How the sampler should filter over mip levels
            samplerDesc.mipmapFilter = _mipsFilterMode;

            samplerDesc.lodMinClamp = 0.0f;
            samplerDesc.lodMaxClamp = (float) mips;
            samplerDesc.compare = wgpu::CompareFunction::Undefined;
            samplerDesc.maxAnisotropy = 1;

            return context->device.createSampler(samplerDesc);
        }
    };

}