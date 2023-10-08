#pragma once

#include "Shader.hpp"

namespace engine {

    class WavesShader : public Shader {
    protected:
        Context *context;

        wgpu::FragmentState fragmentState;
        wgpu::BlendState blendState;
        wgpu::ColorTargetState colorTarget;

        std::vector<VertexAttribute> vertexAttribs;
        VertexBufferLayout vertexBufferLayout;
    public:
        explicit WavesShader(Context *context);
        ~WavesShader() = default;

        wgpu::BindGroupLayout viewProjBindGroupLayout() override;
        wgpu::BindGroupLayout objectBindGroupLayout() override;

        void setAsTarget(RenderPipelineDescriptor &desc) override;
    };

}