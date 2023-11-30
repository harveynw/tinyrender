#pragma once

#include <webgpu/webgpu.hpp>

#include "../../Context.hpp"

wgpu::BindGroupLayout sceneUniformLayout(Context *context);
wgpu::BindGroupLayout texturedModelUniformLayout(Context *context);
wgpu::BindGroupLayout coloredModelUniformLayout(Context *context);
wgpu::BindGroupLayout wavesUniformLayout(Context *context);
wgpu::BindGroupLayout voxelUniformLayout(Context *context);
