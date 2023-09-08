//
// Created by Harvey Williams on 03/08/2023.
//

#include "wgpu_helpers.hpp"

void
helpers::initializeDevice(Instance &instance, Surface &surface, Adapter &adapter, Device &device) {
    /*
     * WebGPU workflow:
     * Request adaptor -> Inspect Capabilities -> Specify Requirements -> Request Device
     */

    std::cout << "Requesting adapter..." << std::endl;
    RequestAdapterOptions adapterOpts;
    adapterOpts.compatibleSurface = surface;
    adapter = instance.requestAdapter(adapterOpts);
    std::cout << "Got adapter: " << adapter << std::endl;

    SupportedLimits supportedLimits;
    adapter.getLimits(&supportedLimits);
    std::cout << "adapter.maxVertexAttributes: " << supportedLimits.limits.maxVertexAttributes << std::endl;

    RequiredLimits requiredLimits = Default;
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
    requiredLimits.limits.maxUniformBuffersPerShaderStage = 2;
    // Update max uniform buffer size:
    requiredLimits.limits.maxUniformBufferBindingSize = 16 * 4 * sizeof(float);
    // 8192px
    requiredLimits.limits.maxTextureDimension1D = 8192;
    requiredLimits.limits.maxTextureDimension2D = 8192;
    requiredLimits.limits.maxTextureArrayLayers = 1;
    requiredLimits.limits.maxSamplersPerShaderStage = 1;
    requiredLimits.limits.maxSampledTexturesPerShaderStage = 1;

    std::cout << "Requesting device..." << std::endl;
    DeviceDescriptor deviceDesc;
    deviceDesc.label = "My Device";
    deviceDesc.requiredFeaturesCount = 0;
    deviceDesc.requiredLimits = &requiredLimits;
    deviceDesc.defaultQueue.label = "The default queue";

    // Finally we request the device with the specified requirements
    device = adapter.requestDevice(deviceDesc);
    std::cout << "Got device: " << device << std::endl;

    // And attach a useful callback
    auto h = device.setUncapturedErrorCallback([](ErrorType type, char const* message) {
        std::cout << "Device error: type " << type;
        if (message) std::cout << " (message: " << message << ")";
        std::cout << std::endl;
    });
}

void
helpers::initializeSwapChain(int width, int height, Surface &surface, Adapter &adapter, Device &device, TextureFormat &swapChainFormat, SwapChain &swapChain) {
    std::cout << "Creating swapchain..." << std::endl;


    // We force this color format for now as Dawn is limited to this one
    (void) adapter;
    swapChainFormat = TextureFormat::BGRA8Unorm;

    /*
    #ifdef WEBGPU_BACKEND_WGPU
        (void) adapter;
        //swapChainFormat = surface.getPreferredFormat(adapter);
        swapChainFormat = TextureFormat::BGRA8Unorm;
    #else
        (void) adapter;
        swapChainFormat = TextureFormat::BGRA8Unorm;
    #endif
    */

    SwapChainDescriptor swapChainDesc;
    swapChainDesc.width = width;
    swapChainDesc.height = height;
    swapChainDesc.usage = TextureUsage::RenderAttachment;
    swapChainDesc.format = swapChainFormat;
    swapChainDesc.presentMode = PresentMode::Fifo;

    // Create Swapchain
    swapChain = device.createSwapChain(surface, swapChainDesc);
    std::cout << "Swapchain: " << swapChain << std::endl;

}
