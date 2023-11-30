//
// Created by Harvey Williams on 08/08/2023.
//

#include "WavesPipeline.hpp"


WavesPipeline::WavesPipeline(Context *context, Scene *scene):
context(context), scene(scene)  {
    std::cout << "Creating render pipeline..." << std::endl;
    RenderPipelineDescriptor desc;

    initialiseShader(desc);
    initialisePipelineOptions(desc);
    initialiseDepthStencil(desc);

    desc.layout = scene->shaders[Waves]->pipelineLayout();

    pipeline = context->device.createRenderPipeline(desc);
    std::cout << "Render pipeline: " << pipeline << std::endl;
}

void
WavesPipeline::onFrame(wgpu::TextureView &textureView, wgpu::CommandEncoder &commandEncoder,
                                  std::vector<std::shared_ptr<tinyrender::Object>> &objects) {
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
    auto &bg = scene->bindGroups.at(Waves);
    renderPass.setBindGroup(0, bg, 0, nullptr);

    for(auto & object : objects) {
        if(object->currentTargetPipeline() == Waves)
            object->onDraw(renderPass, 0, 1);
    } 

    renderPass.end();
}

WavesPipeline::~WavesPipeline() {
    // TODO: Bug, you would think layout.release() would be required but crashes
    //layout.release();
    pipeline.release();
}

void
WavesPipeline::initialiseShader(RenderPipelineDescriptor &desc) {
    this->scene->shaders[Waves]->setAsTarget(desc);
}

void
WavesPipeline::initialisePipelineOptions(RenderPipelineDescriptor &desc) {
    desc.primitive.topology = PrimitiveTopology::TriangleList;
    desc.primitive.stripIndexFormat = IndexFormat::Undefined;
    desc.primitive.frontFace = FrontFace::CCW;
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
WavesPipeline::initialiseDepthStencil(RenderPipelineDescriptor &desc) {
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
