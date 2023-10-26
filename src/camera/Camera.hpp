#pragma once

#include <GLFW/glfw3.h>
#include "../webgpu/primitives/buffers/uniforms/ViewProjMatrixUniform.hpp"


class Camera {
protected:
    virtual void updateInternalBuffer() = 0;
public:
    Camera() = default;
    virtual ~Camera() = default;

    std::shared_ptr<engine::ViewProjMatrixUniform> viewProjectionMatrix = nullptr;

    virtual void onFrame(float dt) = 0;
    virtual void enableListen(GLFWwindow *window, std::shared_ptr<engine::ViewProjMatrixUniform> vpMatrix) = 0;
    virtual void onMouseMove(double xpos, double ypos) = 0;
    virtual void onMouseButton(GLFWwindow *window, int button, int action, int mods) = 0;
    virtual void onScroll(double xoffset, double yoffset) = 0;
    virtual void onKeyEvent(int key, int scancode, int action, int mods) = 0;
};
