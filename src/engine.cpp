
#include <GLFW/glfw3.h>
#include <webgpu/webgpu.hpp>

#include "Engine.hpp"
#include "State.hpp"
#include "webgpu/Context.hpp"
#include "webgpu/pipelines/Pipeline.hpp"
#include "webgpu/Scene.hpp"
#include "webgpu/pipelines/TrianglePipeline.hpp"
#include "webgpu/pipelines/TexturedTrianglePipeline.hpp"
#include "webgpu/pipelines/WavesPipeline.hpp"
#include "webgpu/pipelines/VoxelPipeline.hpp"
#include "webgpu/primitives/textures/Texture2D.hpp"
#include "camera/Camera.hpp"
#include "objects/Object.hpp"


Engine::Engine() {
    if (!glfwInit())
        throw std::runtime_error("Couldn't initialise GLFW");
}

void
Engine::launch(int width, int height) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window = glfwCreateWindow(width, height, "TinyRender", NULL, NULL);
    if (!window)
        throw std::runtime_error("Couldn't create GLFW window");
    printf("GLFW window created at %p\n", (void*) window);

    // Receive everything from GLFW
    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, onWindowMouseMove);
    glfwSetMouseButtonCallback(window, onWindowMouseButton);
    glfwSetScrollCallback(window, onWindowScroll);
    glfwSetKeyCallback(window, onKeyAction);

    // Create webgpu resources
    this->context = buildNewContext(window, width, height);
    this->scene = std::make_shared<Scene>(this->context);

    pipelines[ColoredTriangle] = std::make_unique<TrianglePipeline>(this->context.get(), this->scene.get());
    pipelines[TexturedTriangle] = std::make_unique<TexturedTrianglePipeline>(this->context.get(), this->scene.get());
    pipelines[Waves] = std::make_unique<WavesPipeline>(this->context.get(), this->scene.get());
    pipelines[Voxels] = std::make_unique<VoxelPipeline>(this->context.get(), this->scene.get());

    // Important, first pipeline must set a clear value
    pipelines.begin()->second->enableClear(Color{1.0, 1.0, 1.0, 1.0});

    this->state = std::make_shared<State>();

    #ifdef __EMSCRIPTEN__
    EmscriptenFullscreenStrategy strategy;

    strategy.scaleMode = EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_STDDEF;
    strategy.canvasResolutionScaleMode = EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_STDDEF;
    strategy.filteringMode = EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT;
    strategy.canvasResizedCallback = EmscriptenWindowResizedCallback;
    strategy.canvasResizedCallbackUserData = this;   

    emscripten_enter_soft_fullscreen("#canvas", &strategy);
    #else
    glfwSetFramebufferSizeCallback(window, onWindowResize);
    #endif
}

void Engine::onFrame() {
    glfwPollEvents();

    float dt = 0.01;
    this->state->time += dt;
    this->state->frame++;
    this->state->cameraPosition = camera->getPosition();

    for(const auto& obj : objects) {
        obj->onUpdate(*state.get(), dt);
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
    for(auto &entry : pipelines) {
        auto pipeline = entry.second.get();
        pipeline->onFrame(nextTexture, encoder, objects);
    }
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
    // TODO deleted pipeline destructor, is this okay?
    context.reset(); // Calls the context destructor which will free all the core WebGPU resources.
    glfwDestroyWindow(window);
    glfwTerminate();
}

void
Engine::setCamera(std::shared_ptr<tinyrender::Camera> c) {
    camera = c;
    camera->enableListen(window, this->scene->viewProjUniform);
    camera->onFrame(0);
}

void Engine::onResize(int width, int height) {
    printf("tinyrender::onResize to (%i, %i) %p\n", width, height, (void*) this);

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
    auto engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
    if (engine != nullptr) engine->onResize(width, height);
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
    printf("Got key event %i\n", scancode);
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
    engine->onResize((int) width, (int) height);

    return true;
}
#endif