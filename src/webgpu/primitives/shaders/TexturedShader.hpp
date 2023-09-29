#pragma once

#include "Shader.hpp"

namespace engine {

    class TexturedShader : public Shader {
    protected:
        Context *context;

        wgpu::FragmentState fragmentState;
        wgpu::BlendState blendState;
        wgpu::ColorTargetState colorTarget;

        std::vector<VertexAttribute> vertexAttribs;
        VertexBufferLayout vertexBufferLayout;
    public:
        explicit TexturedShader(Context *context);
        ~TexturedShader() = default;

        wgpu::BindGroupLayout viewProjBindGroupLayout() override;
        wgpu::BindGroupLayout objectBindGroupLayout() override;

        void setAsTarget(RenderPipelineDescriptor &desc) override;
    };

}