#pragma once

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include <glm/glm.hpp>

#include <GLFW/glfw3.h>
#include <webgpu/webgpu.hpp>

#include "webgpu/Context.hpp"
#include "webgpu/pipelines/Pipeline.hpp"
#include "webgpu/Scene.hpp"
#include "webgpu/pipelines/TrianglePipeline.hpp"
#include "webgpu/pipelines/TexturedTrianglePipeline.hpp"
#include "controllers/Controller.hpp"
#include "webgpu/pipelines/WavesPipeline.hpp"


// Forward declarations
namespace engine::Texture2D {
    class Texture;
}

class Engine {
public:
    Engine(int width, int height);
    ~Engine();

    void launch(); // Launch GLFW window

    bool isRunning();
    void onFrame();

    void addObject(std::shared_ptr<engine::Object> obj);

    void onResize(int, int);
    void setController(std::shared_ptr<Controller> controller);

    GLFWwindow *getWindow() { return window; };

    std::shared_ptr<Context> getContext() { return context; }
    std::shared_ptr<Scene> getScene() { return scene; }

    int DISPLAY_WIDTH;
    int DISPLAY_HEIGHT;

    std::shared_ptr<Controller> controller = nullptr;

    // Objects in scene
    std::vector<std::shared_ptr<engine::Object>> objects;
protected:
    // GLFW
    GLFWwindow *window;

    // WebGPU
    std::shared_ptr<Context> context = nullptr;
    std::shared_ptr<Scene> scene = nullptr;

    // Pipeline objects
    std::shared_ptr<TrianglePipeline> trianglePipeline = nullptr;
    std::shared_ptr<TexturedTrianglePipeline> texturedTrianglePipeline = nullptr;
    std::shared_ptr<WavesPipeline> wavesPipeline = nullptr;
};

void onWindowResize(GLFWwindow* window, int, int);
void onWindowMouseMove(GLFWwindow* window, double xpos, double ypos);
void onWindowMouseButton(GLFWwindow* window, int button, int action, int mods);
void onWindowScroll(GLFWwindow* window, double xoffset, double yoffset);
void onKeyAction(GLFWwindow* window, int key, int scancode, int action, int mods);
void onWindowTest(GLFWwindow* window, int, int);
