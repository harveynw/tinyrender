//
// Created by Harvey Williams on 20/07/2023.
//

#include "engine.hpp"

#include <memory>
#include "webgpu/primitives/textures/Texture2D.hpp"


Engine::Engine(int width, int height): DISPLAY_WIDTH(width), DISPLAY_HEIGHT(height) {
    if (!glfwInit())
        throw std::runtime_error("Couldn't initialise GLFW");
}

void
Engine::launch() {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window = glfwCreateWindow(DISPLAY_WIDTH, DISPLAY_HEIGHT, "TinyRender - WebGPU (ESC to unfocus)", NULL, NULL);
    if (!window)
        throw std::runtime_error("Couldn't create GLFW window");

    // Receive everything from GLFW
    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, onWindowMouseMove);
    glfwSetMouseButtonCallback(window, onWindowMouseButton);
    glfwSetScrollCallback(window, onWindowScroll);
    glfwSetKeyCallback(window, onKeyAction);
    glfwSetFramebufferSizeCallback(window, onWindowResize);

    // Create webgpu resources
    this->context = buildNewContext(window, DISPLAY_WIDTH, DISPLAY_HEIGHT);

    // TODO: Scene can absorb this complexity
    this->scene = std::make_shared<Scene>();
    this->scene->buildDepthBuffer(this->context);
    this->scene->lightingUniform = std::make_shared<engine::LightingUniform>(this->context.get());
    this->scene->texturedShader = std::make_shared<engine::TexturedShader>(this->context.get());
    this->scene->coloredShader = std::make_shared<engine::ColoredShader>(this->context.get());
    this->scene->buildViewProj(this->context);

    //this->trianglePipeline = std::make_shared<TrianglePipeline>(this->context.get(), this->scene.get());
    this->texturedTrianglePipeline = std::make_shared<TexturedTrianglePipeline>(this->context.get(), this->scene.get());

    // Important
    this->texturedTrianglePipeline->enableClear(Color{1.0, 1.0, 1.0, 1.0});
}

int Engine::onFrame() {
    /*
     * Update controller
     */
    controller->tick();

    /*
     * Get texture view from swapchain
     */
    wgpu::TextureView nextTexture = context->swapChain.getCurrentTextureView();
    if (!nextTexture) {
        std::cerr << "Cannot acquire next swap chain texture" << std::endl;
        return 1;
    }

    wgpu::CommandEncoderDescriptor commandEncoderDesc;
    commandEncoderDesc.label = "Command Encoder";
    wgpu::CommandEncoder encoder = context->device.createCommandEncoder(commandEncoderDesc);

    /*
     * Draw each pipeline
     */
    //trianglePipeline->onFrame(nextTexture, encoder, objects);
    texturedTrianglePipeline->onFrame(nextTexture, encoder, objects);

    /*
     * Finish up
     */
    nextTexture.release();

    wgpu::CommandBufferDescriptor cmdBufferDescriptor;
    cmdBufferDescriptor.label = "Command buffer";
    wgpu::CommandBuffer command = encoder.finish(cmdBufferDescriptor);
    context->queue.submit(command);

    context->swapChain.present();
    #ifdef WEBGPU_BACKEND_DAWN
    context->device.tick(); // Check for pending error callbacks
    #endif

    return 0;
}

Engine::~Engine() {
    context.reset(); // Calls the context destructor which will free all the WebGPU resources.
    glfwDestroyWindow(window);
    glfwTerminate();
}

void
Engine::setController(std::shared_ptr<Controller> c) {
    controller = c;
    controller->enableListen(window, this->scene->viewProjUniform);
    controller->tick();
}

[[maybe_unused]] int
Engine::enterMainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        int result = onFrame();
        if(result != 0)
            return result;
    }

    return 0;
}

void Engine::onResize(int width, int height) {
    printf("Resize to (%i, %i)\n", width, height);
    // Update fields
    DISPLAY_WIDTH = width;
    DISPLAY_HEIGHT = height;
    context->DISPLAY_WIDTH = width;
    context->DISPLAY_HEIGHT = height;

    context->buildSwapChain();
    scene->buildDepthBuffer(context);
    if(controller != nullptr)
        controller->viewProjectionMatrix->refreshProjectionMatrix(context.get());
}

void
onWindowResize(GLFWwindow* window, int width, int height) {
    auto that = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
    if (that != nullptr) that->onResize(width, height);
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
