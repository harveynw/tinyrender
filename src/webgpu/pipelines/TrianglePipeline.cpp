#include "TrianglePipeline.hpp"


TrianglePipeline::TrianglePipeline(Context *context, Scene *scene):
    context(context), scene(scene) {

    std::cout << "Creating render pipeline..." << std::endl;
    RenderPipelineDescriptor desc;

    initialiseShader(desc);
    initialisePipelineOptions(desc);
    initialiseDepthStencil(desc);

    desc.layout = scene->coloredShader->pipelineLayout();

    pipeline = context->device.createRenderPipeline(desc);
    std::cout << "Render pipeline: " << pipeline << std::endl;
}

void
TrianglePipeline::onFrame(wgpu::TextureView &textureView, wgpu::CommandEncoder &commandEncoder,
                          std::vector<std::shared_ptr<engine::Object>> &objects) {
    RenderPassDescriptor renderPassDesc;

    RenderPassColorAttachment renderPassColorAttachment;
    renderPassColorAttachment.view = textureView;
    renderPassColorAttachment.resolveTarget = nullptr;
    // Op to decided what to do with existing value, can be Clear
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
     * Begin render pass
     */
    RenderPassEncoder renderPass = commandEncoder.beginRenderPass(renderPassDesc);
    renderPass.setPipeline(pipeline);

    // Set the first bind group, won't change between objects
    renderPass.setBindGroup(0, this->scene->coloredViewProjBindGroup, 0, nullptr);

    for(auto & object : objects) {
        if(object->currentTargetPipeline() != ColoredTriangle)
            continue;

        renderPass.setVertexBuffer(0, object->resources->attributeBuffer->getUnderlyingBuffer(), 0, object->resources->attributeBuffer->getSize());
        renderPass.setBindGroup(1, object->resources->bindGroup, 0, nullptr);
        renderPass.draw(object->resources->attributeBuffer->getNDrawCalls(), 1, 0, 0);
    }

    renderPass.end();
}

TrianglePipeline::~TrianglePipeline() {
    // Intermediate resources
    layout.release();
    pipeline.release();
}

void
TrianglePipeline::initialiseShader(RenderPipelineDescriptor &desc) {
    this->scene->coloredShader->setAsTarget(desc);
}

void
TrianglePipeline::initialisePipelineOptions(RenderPipelineDescriptor &desc) {
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
TrianglePipeline::initialiseDepthStencil(RenderPipelineDescriptor &desc) {
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
