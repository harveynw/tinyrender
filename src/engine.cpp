//
// Created by Harvey Williams on 20/07/2023.
//

#include "engine.hpp"

Engine::Engine(int width, int height): DISPLAY_WIDTH(width), DISPLAY_HEIGHT(height) {
    instance = wgpu::createInstance(wgpu::InstanceDescriptor{});
    if (!instance)
        throw std::runtime_error("Couldn't initialise WebGPU");
    if (!glfwInit())
        throw std::runtime_error("Couldn't initialise GLFW");
}

void
Engine::launch() {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(DISPLAY_WIDTH, DISPLAY_HEIGHT, "TinyRender - WebGPU", NULL, NULL);
    if (!window)
        throw std::runtime_error("Couldn't create GLFW window");

    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, onWindowMouseMove);
    glfwSetMouseButtonCallback(window, onWindowMouseButton);
    glfwSetScrollCallback(window, onWindowScroll);
    glfwSetKeyCallback(window, onKeyAction);
    // TODO : glfwSetFramebufferSizeCallback(window, onWindowResize);

    surface = glfwGetWGPUSurface(instance, window);

    helpers::initializeDevice(instance, surface, adapter, device);
    helpers::initializeSwapChain(DISPLAY_WIDTH, DISPLAY_HEIGHT, surface, adapter, device, swapChainFormat, swapChain);

    // Init queue
    queue = device.getQueue();
}

int Engine::onFrame() {
    /*
     * Update controller
     */
    controller->tick();

    /*
     * Get texture view from swapchain
     */
    TextureView nextTexture = this->swapChain.getCurrentTextureView();
    if (!nextTexture) {
        std::cerr << "Cannot acquire next swap chain texture" << std::endl;
        return 1;
    }

    CommandEncoderDescriptor commandEncoderDesc;
    commandEncoderDesc.label = "Command Encoder";
    CommandEncoder encoder = this->device.createCommandEncoder(commandEncoderDesc);

    /*
     * Draw each pipeline
     */
    for(const auto& p: pipelines)
        p->onFrame(nextTexture, encoder);

    /*
     * Finish up
     */
    nextTexture.release();

    CommandBufferDescriptor cmdBufferDescriptor;
    cmdBufferDescriptor.label = "Command buffer";
    CommandBuffer command = encoder.finish(cmdBufferDescriptor);
    this->queue.submit(command);

    this->swapChain.present();
    #ifdef WEBGPU_BACKEND_DAWN
    this->device.tick(); // Check for pending error callbacks
    #endif

    return 0;
}

Engine::~Engine() {
    swapChain.release();
    queue.release();
    device.release();
    adapter.release();
    surface.release();
    instance.release();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void
Engine::setController(std::shared_ptr<Controller> c) {
    controller = c;
    controller->enableListen(window);
    controller->tick();
}

int
Engine::enterMainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        int result = onFrame();
        if(result != 0)
            return result;
    }

    return 0;
}

void
Engine::addPipeline(const std::shared_ptr<Pipeline>& pipeline) {
    pipelines.push_back(pipeline);
}

void
onWindowMouseMove(GLFWwindow* window, double xpos, double ypos) {
    auto engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
    if (engine != nullptr && engine->controller != nullptr) engine->controller->onMouseMove(xpos, ypos);
}

void
onWindowMouseButton(GLFWwindow* window, int button, int action, int mods) {
    auto engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
    if (engine != nullptr && engine->controller != nullptr) engine->controller->onMouseButton(window, button, action, mods);
}

void
onWindowScroll(GLFWwindow* window, double xoffset, double yoffset) {
    auto engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
    if (engine != nullptr && engine->controller != nullptr) engine->controller->onScroll(xoffset, yoffset);
}

void
onKeyAction(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
    if (engine != nullptr && engine->controller != nullptr) engine->controller->onKeyEvent(key, scancode, action, mods);
}
