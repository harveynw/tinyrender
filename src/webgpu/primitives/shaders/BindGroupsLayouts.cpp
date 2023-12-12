#include "BindGroupsLayouts.hpp"


wgpu::BindGroupLayout
sceneUniformLayout(Context *context) {
    std::vector<wgpu::BindGroupLayoutEntry> entries(2, wgpu::Default);
    wgpu::BindGroupLayoutDescriptor bindGroupLayoutDesc{};

    /*
     * Scene: Lighting + View*Projection Matrix
     */
    wgpu::BindGroupLayoutEntry& viewProjectionBindingLayout = entries[0];
    viewProjectionBindingLayout.binding = 0;
    viewProjectionBindingLayout.visibility = wgpu::ShaderStage::Vertex | wgpu::ShaderStage::Fragment;
    viewProjectionBindingLayout.buffer.type = wgpu::BufferBindingType::Uniform;
    viewProjectionBindingLayout.buffer.minBindingSize = sizeof(ViewProjectionUniforms);

    wgpu::BindGroupLayoutEntry& lightingBindingLayout = entries[1];
    lightingBindingLayout.binding = 1;
    lightingBindingLayout.visibility = wgpu::ShaderStage::Fragment;
    lightingBindingLayout.buffer.type = wgpu::BufferBindingType::Uniform;
    lightingBindingLayout.buffer.minBindingSize = sizeof(LightingUniforms);

    bindGroupLayoutDesc.entryCount = (uint32_t) entries.size();
    bindGroupLayoutDesc.entries = entries.data();
    return context->device.createBindGroupLayout(bindGroupLayoutDesc);
}

wgpu::BindGroupLayout
texturedModelUniformLayout(Context *context) {
    std::vector<wgpu::BindGroupLayoutEntry> entries(3, wgpu::Default);
    wgpu::BindGroupLayoutDescriptor bindGroupLayoutDesc{};

    /*
     * Textured Model: Model matrix + Texture + Sampler
     */
    wgpu::BindGroupLayoutEntry& modelBindingLayout = entries[0];
    modelBindingLayout.binding = 0;
    modelBindingLayout.visibility = wgpu::ShaderStage::Vertex | wgpu::ShaderStage::Fragment;
    modelBindingLayout.buffer.type = wgpu::BufferBindingType::Uniform;
    modelBindingLayout.buffer.minBindingSize = sizeof(ModelUniform);

    wgpu::BindGroupLayoutEntry& textureBindingLayout = entries[1];
    textureBindingLayout.binding = 1;
    textureBindingLayout.visibility = wgpu::ShaderStage::Fragment;
    textureBindingLayout.texture.sampleType = wgpu::TextureSampleType::Float; // Because using normalised format
    textureBindingLayout.texture.viewDimension = wgpu::TextureViewDimension::_2D;

    wgpu::BindGroupLayoutEntry& samplerBindingLayout = entries[2];
    samplerBindingLayout.binding = 2;
    samplerBindingLayout.visibility = wgpu::ShaderStage::Fragment;
    samplerBindingLayout.sampler.type = wgpu::SamplerBindingType::Filtering;

    bindGroupLayoutDesc.entryCount = (uint32_t) entries.size();
    bindGroupLayoutDesc.entries = entries.data();
    return context->device.createBindGroupLayout(bindGroupLayoutDesc);
}

wgpu::BindGroupLayout
coloredModelUniformLayout(Context *context) {
    std::vector<wgpu::BindGroupLayoutEntry> entries(2, wgpu::Default);
    wgpu::BindGroupLayoutDescriptor bindGroupLayoutDesc{};

    /*
     * Colored Model: Model matrix + color
     */

    wgpu::BindGroupLayoutEntry &modelBindingLayout = entries[0];
    modelBindingLayout.binding = 0;
    modelBindingLayout.visibility = wgpu::ShaderStage::Vertex | wgpu::ShaderStage::Fragment;
    modelBindingLayout.buffer.type = wgpu::BufferBindingType::Uniform;
    modelBindingLayout.buffer.minBindingSize = sizeof(ModelUniform);

    wgpu::BindGroupLayoutEntry &colorBindingLayout = entries[1];
    colorBindingLayout.binding = 1;
    colorBindingLayout.visibility = wgpu::ShaderStage::Fragment;
    colorBindingLayout.buffer.type = wgpu::BufferBindingType::Uniform;
    colorBindingLayout.buffer.minBindingSize = sizeof(ColorUniforms);

    bindGroupLayoutDesc.entryCount = (uint32_t) entries.size();
    bindGroupLayoutDesc.entries = entries.data();
    return context->device.createBindGroupLayout(bindGroupLayoutDesc);
}

wgpu::BindGroupLayout
wavesUniformLayout(Context *context) {
    std::vector<wgpu::BindGroupLayoutEntry> entries(5, wgpu::Default);
    wgpu::BindGroupLayoutDescriptor bindGroupLayoutDesc{};

    /*
     * Waves: Model matrix + simulation data
     */
    wgpu::BindGroupLayoutEntry &modelBindingLayout = entries[0];
    modelBindingLayout.binding = 0;
    modelBindingLayout.visibility = wgpu::ShaderStage::Vertex;
    modelBindingLayout.buffer.type = wgpu::BufferBindingType::Uniform;
    modelBindingLayout.buffer.minBindingSize = sizeof(ModelUniform);

    wgpu::BindGroupLayoutEntry& textureBindingLayout = entries[1];
    textureBindingLayout.binding = 1;
    textureBindingLayout.visibility = wgpu::ShaderStage::Vertex | wgpu::ShaderStage::Fragment;
    textureBindingLayout.texture.sampleType = wgpu::TextureSampleType::Float; // Because using normalised format
    textureBindingLayout.texture.viewDimension = wgpu::TextureViewDimension::_2D;

    wgpu::BindGroupLayoutEntry& samplerBindingLayout = entries[2];
    samplerBindingLayout.binding = 2;
    samplerBindingLayout.visibility = wgpu::ShaderStage::Vertex | wgpu::ShaderStage::Fragment;
    samplerBindingLayout.sampler.type = wgpu::SamplerBindingType::Filtering;

    wgpu::BindGroupLayoutEntry& maxDispBindingLayout = entries[3];
    maxDispBindingLayout.binding = 3;
    maxDispBindingLayout.visibility = wgpu::ShaderStage::Vertex | wgpu::ShaderStage::Fragment;
    maxDispBindingLayout.buffer.type = wgpu::BufferBindingType::Uniform;
    maxDispBindingLayout.buffer.minBindingSize = sizeof(ScalarUniforms);

    wgpu::BindGroupLayoutEntry &colorBindingLayout = entries[4];
    colorBindingLayout.binding = 4;
    colorBindingLayout.visibility = wgpu::ShaderStage::Vertex;
    colorBindingLayout.buffer.type = wgpu::BufferBindingType::Uniform;
    colorBindingLayout.buffer.minBindingSize = sizeof(ColorUniforms);

    bindGroupLayoutDesc.entryCount = (uint32_t) entries.size();
    bindGroupLayoutDesc.entries = entries.data();
    return context->device.createBindGroupLayout(bindGroupLayoutDesc);
}

wgpu::BindGroupLayout
voxelUniformLayout(Context *context) {
    std::vector<wgpu::BindGroupLayoutEntry> entries(4, wgpu::Default);
    wgpu::BindGroupLayoutDescriptor bindGroupLayoutDesc{};

    /*
     * Voxels: Chunk model matrix + Overall voxel object model matrix
     */
    wgpu::BindGroupLayoutEntry &voxelsModelBindingLayout = entries[0];
    voxelsModelBindingLayout.binding = 0;
    voxelsModelBindingLayout.visibility = wgpu::ShaderStage::Vertex;
    voxelsModelBindingLayout.buffer.type = wgpu::BufferBindingType::Uniform;
    voxelsModelBindingLayout.buffer.minBindingSize = sizeof(ModelUniform);

    wgpu::BindGroupLayoutEntry &chunkModelBindingLayout = entries[1];
    chunkModelBindingLayout.binding = 1;
    chunkModelBindingLayout.visibility = wgpu::ShaderStage::Vertex;
    chunkModelBindingLayout.buffer.type = wgpu::BufferBindingType::Uniform;
    chunkModelBindingLayout.buffer.minBindingSize = sizeof(ModelUniform);
    
    wgpu::BindGroupLayoutEntry& textureBindingLayout = entries[2];
    textureBindingLayout.binding = 2;
    textureBindingLayout.visibility = wgpu::ShaderStage::Vertex | wgpu::ShaderStage::Fragment;
    textureBindingLayout.texture.sampleType = wgpu::TextureSampleType::Float; // Because using normalised format
    textureBindingLayout.texture.viewDimension = wgpu::TextureViewDimension::_2D;

    wgpu::BindGroupLayoutEntry& samplerBindingLayout = entries[3];
    samplerBindingLayout.binding = 3;
    samplerBindingLayout.visibility = wgpu::ShaderStage::Vertex | wgpu::ShaderStage::Fragment;
    samplerBindingLayout.sampler.type = wgpu::SamplerBindingType::Filtering;


    bindGroupLayoutDesc.entryCount = (uint32_t) entries.size();
    bindGroupLayoutDesc.entries = entries.data();
    return context->device.createBindGroupLayout(bindGroupLayoutDesc);
}
