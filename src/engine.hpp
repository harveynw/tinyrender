#pragma once

#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>
#include <webgpu/webgpu.hpp>

#include "webgpu/Context.hpp"
#include "webgpu/pipelines/Pipeline.hpp"
#include "controllers/Controller.hpp"
#include "webgpu/Scene.hpp"


// Forward declarations
namespace engine::Texture2D {
    class Texture;
}

class Engine {
public:
    Engine(int width, int height);
    ~Engine();

    void launch(); // Launch GLFW window
    [[maybe_unused]] int enterMainLoop();

    int onFrame(); // internal call from main event loop

    void onResize(int, int);

    void setController(std::shared_ptr<Controller> controller);
    void addPipeline(const std::shared_ptr<Pipeline>& pipeline);

    GLFWwindow *getWindow() { return window; };

    std::shared_ptr<Context> getContext() { return context; }
    std::shared_ptr<Scene> getScene() { return scene; }

    int DISPLAY_WIDTH;
    int DISPLAY_HEIGHT;

    std::shared_ptr<Controller> controller = nullptr;
protected:
    // GLFW
    GLFWwindow *window;

    // WebGPU
    std::shared_ptr<Context> context = nullptr;
    std::shared_ptr<Scene> scene = nullptr;

    // Pipeline objects
    std::vector<std::shared_ptr<Pipeline>> pipelines;
};

void onWindowResize(GLFWwindow* window, int, int);
void onWindowMouseMove(GLFWwindow* window, double xpos, double ypos);
void onWindowMouseButton(GLFWwindow* window, int button, int action, int mods);
void onWindowScroll(GLFWwindow* window, double xoffset, double yoffset);
void onKeyAction(GLFWwindow* window, int key, int scancode, int action, int mods);
