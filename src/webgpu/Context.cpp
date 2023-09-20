#include "Context.hpp"

std::shared_ptr<Context>
buildNewContext(GLFWwindow *window, int width, int height) {
    auto context = std::make_shared<Context>();
    context->DISPLAY_WIDTH = width;
    context->DISPLAY_HEIGHT = height;

    /*
     * First step: Create a context and get a surface from the GLFW window.
     */
    context->instance = wgpu::createInstance(wgpu::InstanceDescriptor{});
    if (!context->instance)
        throw std::runtime_error("Couldn't initialise WebGPU");
    context->surface = glfwGetWGPUSurface(context->instance, window);

    /*
     * WebGPU workflow:
     * Request adaptor -> Inspect Capabilities -> Specify Requirements -> Request Device
     */
    std::cout << "Requesting adapter..." << std::endl;
    wgpu::RequestAdapterOptions adapterOpts;
    adapterOpts.compatibleSurface = context->surface;
    context->adapter = context->instance.requestAdapter(adapterOpts);

    wgpu::SupportedLimits supportedLimits;
    context->adapter.getLimits(&supportedLimits);
    //std::cout << "adapter.maxVertexAttributes: " << supportedLimits.limits.maxVertexAttributes << std::endl;

    wgpu::RequiredLimits requiredLimits = wgpu::Default;
    requiredLimits.limits.maxVertexAttributes = 4;
    requiredLimits.limits.maxVertexBuffers = 1;

    //uint64_t n_verts = 8589934592; // for the bugatti
    //requiredLimits.limits.maxBufferSize = n_verts;
    int attributeExpectedSize = 44;
    requiredLimits.limits.maxBufferSize = 10000000 * attributeExpectedSize;
    requiredLimits.limits.maxVertexBufferArrayStride = attributeExpectedSize;
    requiredLimits.limits.minStorageBufferOffsetAlignment = supportedLimits.limits.minStorageBufferOffsetAlignment;
    requiredLimits.limits.minUniformBufferOffsetAlignment = supportedLimits.limits.minUniformBufferOffsetAlignment;
    requiredLimits.limits.maxInterStageShaderComponents = 8;
    requiredLimits.limits.maxBindGroups = 2;
    requiredLimits.limits.maxUniformBuffersPerShaderStage = 3;
    // Update max uniform buffer size:
    requiredLimits.limits.maxUniformBufferBindingSize = 16 * 4 * sizeof(float);
    // 8192px
    requiredLimits.limits.maxTextureDimension1D = 8192;
    requiredLimits.limits.maxTextureDimension2D = 8192;
    requiredLimits.limits.maxTextureArrayLayers = 1;
    requiredLimits.limits.maxSamplersPerShaderStage = 1;
    requiredLimits.limits.maxSampledTexturesPerShaderStage = 1;

    std::cout << "Requesting device..." << std::endl;
    wgpu::DeviceDescriptor deviceDesc;
    deviceDesc.label = "My Device";
    deviceDesc.requiredFeaturesCount = 0;
    deviceDesc.requiredLimits = &requiredLimits;
    deviceDesc.defaultQueue.label = "The default queue";

    // Finally we request the device with the specified requirements
    context->device = context->adapter.requestDevice(deviceDesc);

    // And attach a useful callback
    auto h = context->device.setUncapturedErrorCallback([](wgpu::ErrorType type, char const* message) {
        std::cout << "Device error: type " << type;
        if (message) std::cout << " (message: " << message << ")";
        std::cout << std::endl;
    });

    // We force this color format for now as Dawn is limited to this one
    context->swapChainFormat = wgpu::TextureFormat::BGRA8Unorm;
    //swapChainFormat = surface.getPreferredFormat(adapter);

    /*
     * Final step: Create the swapchain so we can output to screen.
     */
    context->buildSwapChain();

    context->queue = context->device.getQueue();
    context->print();
    return context;
}

void Context::print() {
    std::cout << "-- WebGPU Context --" << std::endl;
    std::cout << "Adapter: " << this->adapter << std::endl;
    std::cout << "Device: " << this->device << std::endl;
    std::cout << "Swap-chain: " << this->swapChain << std::endl;
    std::cout << "--" << std::endl;
}

void Context::buildSwapChain() {
    if(swapChain != nullptr)
        swapChain.release();

    std::cout << "Creating swapchain..." << std::endl;
    wgpu::SwapChainDescriptor swapChainDesc;
    swapChainDesc.width = this->DISPLAY_WIDTH;
    swapChainDesc.height = this->DISPLAY_HEIGHT;
    swapChainDesc.usage = wgpu::TextureUsage::RenderAttachment;
    swapChainDesc.format = this->swapChainFormat;
    swapChainDesc.presentMode = wgpu::PresentMode::Fifo;

    this->swapChain = this->device.createSwapChain(this->surface, swapChainDesc);
}

Context::~Context() {
    swapChain.release();
    queue.release();
    device.release();
    adapter.release();
    surface.release();
    instance.release();
}
