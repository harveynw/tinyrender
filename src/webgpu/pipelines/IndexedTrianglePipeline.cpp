//
// Created by Harvey Williams on 08/08/2023.
//

#include "IndexedTrianglePipeline.hpp"
#include "../load_shader.hpp"


IndexedTrianglePipeline::IndexedTrianglePipeline(Context *context,
                                                 Scene *scene,
                                                 std::shared_ptr<engine::UniformViewProjection> uniforms,
                                                 std::vector<IndexedTriangleObject> &objects):
context(context), scene(scene) {
    this->uniforms = uniforms;
    this->objects = objects;

    initialiseBuffers();

    std::cout << "Creating render pipeline..." << std::endl;
    RenderPipelineDescriptor desc;

    initialiseShader(desc);
    initialisePipelineOptions(desc);
    initialiseDepthBufferAndStencil(desc);
    initialiseAttributes(desc);
    initialiseUniformBindGroup(desc);

    pipeline = context->device.createRenderPipeline(desc);
    std::cout << "Render pipeline: " << pipeline << std::endl;
}

void
IndexedTrianglePipeline::onFrame(wgpu::TextureView &textureView, wgpu::CommandEncoder &commandEncoder) {
    /*
     * Step 1: Update uniforms
     */
    //uniforms->updateRotationAboutZAxis();

    /*
     * Step 2: Create Render Pass
     */

    RenderPassDescriptor renderPassDesc;

    RenderPassColorAttachment renderPassColorAttachment;
    renderPassColorAttachment.view = textureView;
    renderPassColorAttachment.resolveTarget = nullptr;
    renderPassColorAttachment.loadOp = shouldClear ? LoadOp::Clear : LoadOp::Load;
    renderPassColorAttachment.storeOp = StoreOp::Store;
    renderPassColorAttachment.clearValue = shouldClear ? clearValue : Color{0, 0, 0, 0};
    renderPassDesc.colorAttachmentCount = 1;
    renderPassDesc.colorAttachments = &renderPassColorAttachment; // Color attachment
    renderPassDesc.timestampWriteCount = 0;
    renderPassDesc.timestampWrites = nullptr;

    RenderPassDepthStencilAttachment depthStencilAttachment;
    // The view of the depth texture
    depthStencilAttachment.view = scene->depthTexture->getView();

    // The initial value of the depth buffer, meaning "far"
    depthStencilAttachment.depthClearValue = 1.0f;
    // Operation settings comparable to the color attachment
    depthStencilAttachment.depthLoadOp = shouldClear ? LoadOp::Clear : LoadOp::Load;
    depthStencilAttachment.depthStoreOp = StoreOp::Store;
    depthStencilAttachment.depthReadOnly = false; // we could turn off writing to the depth buffer globally here
    // Stencil setup, mandatory but unused

    depthStencilAttachment.stencilClearValue = 0;
    #ifdef WEBGPU_BACKEND_WGPU
    depthStencilAttachment.stencilLoadOp = LoadOp::Clear;
    depthStencilAttachment.stencilStoreOp = StoreOp::Store;
    #else
    depthStencilAttachment.stencilLoadOp = LoadOp::Undefined;
    depthStencilAttachment.stencilStoreOp = StoreOp::Undefined;
    #endif

    depthStencilAttachment.stencilReadOnly = true;
    renderPassDesc.depthStencilAttachment = &depthStencilAttachment; // Depth Stencil Attachment

    /*
     * Step 3: Set attribute locations to buffers and perform draw call
     */
    RenderPassEncoder renderPass = commandEncoder.beginRenderPass(renderPassDesc);
    // In its overall outline, drawing a triangle is as simple as this:
    // Select which render pipeline to use
    renderPass.setPipeline(pipeline);

    auto obj_it = objects.begin();
    auto bindgroup_it = bindGroups.begin();
    while(obj_it != objects.end()) {
        // Set vertex buffer
        renderPass.setVertexBuffer(0, obj_it->vertexData->getUnderlyingBuffer(), 0, obj_it->vertexData->getSize());
        // Set index buffer
        renderPass.setIndexBuffer(obj_it->indexData->getUnderlyingBuffer(), IndexFormat::Uint16, 0, obj_it->indexData->getSize());
        // Set to the bind group that points to the corresponding model matrix
        renderPass.setBindGroup(0, *bindgroup_it, 0, nullptr);
        // draw call
        renderPass.drawIndexed(obj_it->indexData->getNDrawCalls(), 1, 0, 0, 0);

        obj_it++;
        bindgroup_it++;
    }

    /*
    // Set vertex buffers while encoding the render pass
    renderPass.setVertexBuffer(0, vertexBuffer, 0, data.vertexData.size() * sizeof(float));
    renderPass.setIndexBuffer(indexBuffer, IndexFormat::Uint16, 0,
                              data.indexData.size() * sizeof(uint16_t));

    // bind uniform
    renderPass.setBindGroup(0, bindGroup, 0, nullptr);

    // indexed draw call
    renderPass.drawIndexed(data.indexData.size(), 1, 0, 0, 0);
    */

    renderPass.end();
}

IndexedTrianglePipeline::~IndexedTrianglePipeline() {
    for(auto bg : bindGroups)
        bg.release();

    // Intermediate resources
    shaderModule.release();
    layout.release();
    bindGroupLayout.release();
}

void
IndexedTrianglePipeline::initialiseBuffers() {}

void
IndexedTrianglePipeline::initialiseShader(RenderPipelineDescriptor &desc) {
    shaderModule = loadShaderModule("resources/shaders/basic_mvp.wgsl", context->device);
    std::cout << "Shader module: " << shaderModule << std::endl;

    // Vertex shader
    desc.vertex.module = shaderModule;
    desc.vertex.entryPoint = "vs_main";
    desc.vertex.constantCount = 0;
    desc.vertex.constants = nullptr;

    // Fragment shader
    desc.fragment = &fragmentState;
    fragmentState.module = shaderModule;
    fragmentState.entryPoint = "fs_main";
    fragmentState.constantCount = 0;
    fragmentState.constants = nullptr;

    // Configure blend state
    // Usual alpha blending for the color:
    blendState.color.srcFactor = BlendFactor::SrcAlpha;
    blendState.color.dstFactor = BlendFactor::OneMinusSrcAlpha;
    blendState.color.operation = BlendOperation::Add;
    // We leave the target alpha untouched:
    blendState.alpha.srcFactor = BlendFactor::Zero;
    blendState.alpha.dstFactor = BlendFactor::One;
    blendState.alpha.operation = BlendOperation::Add;

    colorTarget.format = context->swapChainFormat;
    colorTarget.blend = &blendState;
    colorTarget.writeMask = ColorWriteMask::All; // We could write to only some of the color channels.

    // We have only one target because our render pass has only one output color
    // attachment.
    fragmentState.targetCount = 1;
    fragmentState.targets = &colorTarget;
}

void
IndexedTrianglePipeline::initialisePipelineOptions(RenderPipelineDescriptor &desc) {
    // Primitive assembly and rasterization
    // Each sequence of 3 vertices is considered as a triangle
    desc.primitive.topology = PrimitiveTopology::TriangleList;
    // We'll see later how to specify the order in which vertices should be
    // connected. When not specified, vertices are considered sequentially.
    desc.primitive.stripIndexFormat = IndexFormat::Undefined;
    // The face orientation is defined by assuming that when looking
    // from the front of the face, its corner vertices are enumerated
    // in the counter-clockwise (CCW) order.
    desc.primitive.frontFace = FrontFace::CCW;
    // But the face orientation does not matter much because we do not
    // cull (i.e. "hide") the faces pointing away from us (which is often
    // used for optimization).
    desc.primitive.cullMode = CullMode::None;

    // Multi-sampling
    // Samples per pixel
    desc.multisample.count = 1;
    // Default value for the mask, meaning "all bits on"
    desc.multisample.mask = ~0u;
    // Default value as well (irrelevant for count = 1 anyways)
    desc.multisample.alphaToCoverageEnabled = false;
}

void
IndexedTrianglePipeline::initialiseDepthBufferAndStencil(RenderPipelineDescriptor &desc) {
    // We setup a depth buffer state for the render pipeline
    // Keep a fragment only if its depth is lower than the previously blended one
    depthStencilState.depthCompare = CompareFunction::Less;
    // Each time a fragment is blended into the target, we update the value of the Z-buffer
    depthStencilState.depthWriteEnabled = true;
    // Store the format in a variable as later parts of the code depend on it
    depthTextureFormat = TextureFormat::Depth24Plus;
    depthStencilState.format = depthTextureFormat;
    // Deactivate the stencil alltogether
    depthStencilState.stencilReadMask = 0;
    depthStencilState.stencilWriteMask = 0;

    desc.depthStencil = &depthStencilState;
}

void
IndexedTrianglePipeline::initialiseAttributes(RenderPipelineDescriptor &desc) {
    vertexAttribs = std::vector<VertexAttribute>(3);

    // Position attribute
    vertexAttribs[0].shaderLocation = 0;
    vertexAttribs[0].format = VertexFormat::Float32x3;
    vertexAttribs[0].offset = offsetof(IndexedTriangleVertexAttributes, position);

    // Normal attribute
    vertexAttribs[1].shaderLocation = 1;
    vertexAttribs[1].format = VertexFormat::Float32x3;
    vertexAttribs[1].offset = offsetof(IndexedTriangleVertexAttributes, normal);

    // Color attribute
    vertexAttribs[2].shaderLocation = 2;
    vertexAttribs[2].format = VertexFormat::Float32x3;
    vertexAttribs[2].offset = offsetof(IndexedTriangleVertexAttributes, color);

    // Single vertex buffer layout (specified by IndexedTriangleVertexAttributes)
    vertexBufferLayout.attributeCount = (uint32_t)vertexAttribs.size();
    vertexBufferLayout.attributes = vertexAttribs.data();
    vertexBufferLayout.arrayStride = sizeof(IndexedTriangleVertexAttributes);
    vertexBufferLayout.stepMode = VertexStepMode::Vertex;

    // Specify that we have one buffer and this is the layout
    desc.vertex.bufferCount = 1;
    desc.vertex.buffers = &vertexBufferLayout;
}

void
IndexedTrianglePipeline::initialiseUniformBindGroup(RenderPipelineDescriptor &desc) {
    /*
     * Uniforms require setting up a Bind Group accessed through the shader
     */
    bindingLayoutEntries = std::vector<BindGroupLayoutEntry>(2, Default);

    // Setup uniforms binding
    BindGroupLayoutEntry& viewProjectionBindingLayout = bindingLayoutEntries[0];
    viewProjectionBindingLayout.binding = 0;
    viewProjectionBindingLayout.visibility = ShaderStage::Vertex | ShaderStage::Fragment;
    viewProjectionBindingLayout.buffer.type = BufferBindingType::Uniform;
    viewProjectionBindingLayout.buffer.minBindingSize = sizeof(engine::ViewProjectionUniforms);

    BindGroupLayoutEntry& modelBindingLayout = bindingLayoutEntries[1];
    modelBindingLayout.binding = 1;
    modelBindingLayout.visibility = ShaderStage::Vertex | ShaderStage::Fragment;
    modelBindingLayout.buffer.type = BufferBindingType::Uniform;
    modelBindingLayout.buffer.minBindingSize = sizeof(engine::ModelUniform);

    // Create a bind group layout
    BindGroupLayoutDescriptor bindGroupLayoutDesc{};
    bindGroupLayoutDesc.entryCount = (uint32_t) bindingLayoutEntries.size();
    bindGroupLayoutDesc.entries = bindingLayoutEntries.data();
    bindGroupLayout = context->device.createBindGroupLayout(bindGroupLayoutDesc);

    // Create the pipeline layout and set it in render pipeline descriptor
    PipelineLayoutDescriptor layoutDesc{};
    layoutDesc.bindGroupLayoutCount = 1;
    layoutDesc.bindGroupLayouts = (WGPUBindGroupLayout*) &bindGroupLayout;
    layout = context->device.createPipelineLayout(layoutDesc);
    desc.layout = layout;


    /*
     * Bind Group Creation
     */
    for(const auto &obj : objects) {
        bindGroupEntries.emplace_back(2);

        std::vector<BindGroupEntry> &bindings = bindGroupEntries.back();
        bindings[0] = uniforms->generateUniformBindGroupEntry(0); // uniforms
        bindings[1] = obj.modelMatrix->generateUniformBindGroupEntry(1);

        BindGroupDescriptor bindGroupDesc;
        bindGroupDesc.layout = bindGroupLayout;
        bindGroupDesc.entryCount = (uint32_t)bindings.size();
        bindGroupDesc.entries = bindings.data();
        bindGroups.push_back(context->device.createBindGroup(bindGroupDesc));
    }
}
