//
// Created by Harvey Williams on 20/07/2023.
//

#include "engine.hpp"

#include <memory>
#include "webgpu/primitives/textures/Texture2D.hpp"


Engine::Engine() {
    if (!glfwInit())
        throw std::runtime_error("Couldn't initialise GLFW");
}

void
Engine::launch(int width, int height) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window = glfwCreateWindow(width, height, "Engine", NULL, NULL);
    if (!window)
        throw std::runtime_error("Couldn't create GLFW window");
    printf("GLFW window created at %p\n", (void*) window);

    // Receive everything from GLFW
    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, onWindowMouseMove);
    glfwSetMouseButtonCallback(window, onWindowMouseButton);
    glfwSetScrollCallback(window, onWindowScroll);
    glfwSetKeyCallback(window, onKeyAction);
    glfwSetFramebufferSizeCallback(window, onWindowResize);

    /*
    #ifdef __EMSCRIPTEN__
    strategy.scaleMode = EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_STDDEF;
    strategy.filteringMode = EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT;
    strategy.canvasResizedCallback = EmscriptenWindowResizedCallback;
    strategy.canvasResizedCallbackUserData = this;   
    emscripten_enter_soft_fullscreen("#canvas", &strategy);
    #endif
    */

    // Create webgpu resources
    this->context = buildNewContext(window, width, height);

    // TODO: Scene can absorb this complexity
    this->scene = std::make_shared<Scene>();
    this->scene->buildDepthBuffer(this->context);
    this->scene->lightingUniform = std::make_shared<tinyrender::LightingUniform>(this->context.get());
    this->scene->texturedShader = std::make_shared<tinyrender::TexturedShader>(this->context.get());
    this->scene->coloredShader = std::make_shared<tinyrender::ColoredShader>(this->context.get());
    this->scene->wavesShader = std::make_shared<tinyrender::WavesShader>(this->context.get());
    this->scene->buildViewProj(this->context);

    this->trianglePipeline = std::make_shared<TrianglePipeline>(this->context.get(), this->scene.get());
    this->texturedTrianglePipeline = std::make_shared<TexturedTrianglePipeline>(this->context.get(), this->scene.get());
    this->wavesPipeline = std::make_shared<WavesPipeline>(this->context.get(), this->scene.get());

    // Important
    this->trianglePipeline->enableClear(Color{1.0, 1.0, 1.0, 1.0});
}

void Engine::onFrame() {
    glfwPollEvents();

    float dt = 0.01;
    this->state.time += dt;
    this->state.frame++;
    this->state.cameraPosition = camera->getPosition();

    for(const auto& obj : objects) {
        obj->onUpdate(state, dt);
    }

    /*
     * Update camera
     */
    camera->onFrame(dt);

    /*
     * Get texture view from swapchain
     */
    wgpu::TextureView nextTexture = context->swapChain.getCurrentTextureView();
    if (!nextTexture) {
        std::cerr << "Cannot acquire next swap chain texture" << std::endl;
        //return 1;
        return;
    }

    wgpu::CommandEncoderDescriptor commandEncoderDesc;
    commandEncoderDesc.label = "Command Encoder";
    wgpu::CommandEncoder encoder = context->device.createCommandEncoder(commandEncoderDesc);

    /*
     * Draw each pipeline
     */
    trianglePipeline->onFrame(nextTexture, encoder, objects);
    texturedTrianglePipeline->onFrame(nextTexture, encoder, objects);
    wavesPipeline->onFrame(nextTexture, encoder, objects);

    /*
     * Finish up
     */

    nextTexture.release();

    wgpu::CommandBufferDescriptor cmdBufferDescriptor;
    cmdBufferDescriptor.label = "Command buffer";
    wgpu::CommandBuffer command = encoder.finish(cmdBufferDescriptor);
    context->queue.submit(command);

    #ifndef __EMSCRIPTEN__
    // No need if in browser
    context->swapChain.present();
    #endif

    #ifdef WEBGPU_BACKEND_DAWN
    context->device.tick(); // Check for pending error callbacks
    #endif
}

void 
Engine::addObject(std::shared_ptr<tinyrender::Object> obj) {
    obj->onInit(context.get(), scene.get());
    objects.push_back(obj);
}

Engine::~Engine() {
    objects.clear();
    camera.reset();
    scene.reset();

    trianglePipeline.reset();
    texturedTrianglePipeline.reset();

    context.reset(); // Calls the context destructor which will free all the core WebGPU resources.
    glfwDestroyWindow(window);
    glfwTerminate();
}

void
Engine::setCamera(std::shared_ptr<Camera> c) {
    camera = c;
    camera->enableListen(window, this->scene->viewProjUniform);
    camera->onFrame(0);
}

void Engine::onResize(int width, int height) {
    //printf("tinyrender::onResize to (%i, %i) %p\n", width, height, (void*) this);

    // Terminate depth texture
    scene->depthTexture = nullptr;

    // Rebuild swap chain
    context->buildSwapChain(width, height);

    // Rebuild depth texture
    scene->buildDepthBuffer(context);

    if(camera != nullptr)
        camera->viewProjectionMatrix->refreshProjectionMatrix(context.get());
}

bool Engine::isRunning() {
    return !glfwWindowShouldClose(getWindow());
}

void
onWindowResize(GLFWwindow* window, int width, int height) {
    void *ptr = glfwGetWindowUserPointer(window);
    auto that = reinterpret_cast<Engine*>(ptr);
    //printf("GLFW::onWindowResize (%i, %i), %p\n", width, height, ptr);
    if (that != nullptr) that->onResize(width, height);
}

void
onWindowMouseMove(GLFWwindow* window, double xpos, double ypos) {
    auto engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
    if (engine != nullptr && engine->camera != nullptr) engine->camera->onMouseMove(xpos, ypos);
}

void
onWindowMouseButton(GLFWwindow* window, int button, int action, int mods) {
    auto engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
    if (engine != nullptr && engine->camera != nullptr) engine->camera->onMouseButton(window, button, action, mods);
}

void
onWindowScroll(GLFWwindow* window, double xoffset, double yoffset) {
    auto engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
    if (engine != nullptr && engine->camera != nullptr) engine->camera->onScroll(xoffset, yoffset);
}

void
onKeyAction(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
    if (engine != nullptr && engine->camera != nullptr) engine->camera->onKeyEvent(key, scancode, action, mods);
}

#ifdef __EMSCRIPTEN__
EM_BOOL EmscriptenWindowResizedCallback(int eventType, const void *event, void *userData)
{
    (void) eventType; (void) event; 

	double width, height;
	emscripten_get_element_css_size("canvas", &width, &height);
    printf("Emscripten Callback : resize (%f, %f) Engine:%p\n", width, height, userData);

	Engine* engine = (Engine*) userData;
    glfwSetWindowSize(engine->getWindow(), (int) width, (int) height);

    return EM_BOOL();
}
#endif