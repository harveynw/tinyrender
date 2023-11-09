#pragma once

#include <webgpu/webgpu.hpp>

#include "../../Context.hpp"
#include "BindGroupsLayouts.hpp"
#include "load_shader.hpp"
#include "../buffers/AttributeStructs.hpp"

/*
 * Shader
 * Instances encapsulate everything tied to a specific shader (bind group layouts etc.) that one or more pipelines can target.
 */

namespace tinyrender {

    class Shader {
    protected:
        std::vector<wgpu::BindGroupLayout> bindGroupLayouts;
        wgpu::ShaderModule shaderModule = nullptr;
        wgpu::PipelineLayout layout = nullptr;

    public:
        // Pipeline layout needed for pipelines to target
        wgpu::PipelineLayout pipelineLayout() {
            if(layout == nullptr)
                throw std::runtime_error("Pipeline layout requested but not initialised.");
            return layout;
        };

        // Slow bind group layout (lighting + viewProjection)
        virtual wgpu::BindGroupLayout viewProjBindGroupLayout() = 0;

        // The principal bind group layout (usually containing the model matrix) for objects to generate their bind groups against
        virtual wgpu::BindGroupLayout objectBindGroupLayout() = 0;

        // Pipelines can pass there render pipeline descriptor to be populated.
        virtual void setAsTarget(RenderPipelineDescriptor &desc) = 0;

        virtual ~Shader() {
            for (auto bgl: bindGroupLayouts)
                bgl.release();
            layout.release();
            shaderModule.release();
        };
    };

}