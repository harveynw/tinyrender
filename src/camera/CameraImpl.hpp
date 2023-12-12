#pragma once

#include <GLFW/glfw3.h>
#include <memory>

#include "camera/Camera.hpp"
#include "../webgpu/primitives/uniforms/ViewProjMatrixUniform.hpp"

class CameraImpl {
protected:
    virtual void updateInternalBuffer() = 0;
public:
    CameraImpl() = default;
    virtual ~CameraImpl() = default;

    std::shared_ptr<ViewProjMatrixUniform> viewProjectionMatrix = nullptr;

    virtual glm::vec3 getPosition() = 0;

    virtual void onFrame(float dt) = 0;
    virtual void enableListen(GLFWwindow *window, std::shared_ptr<ViewProjMatrixUniform> vpMatrix) = 0;
    virtual void onMouseMove(double xpos, double ypos) = 0;
    virtual void onMouseButton(GLFWwindow *window, int button, int action, int mods) = 0;
    virtual void onScroll(double xoffset, double yoffset) = 0;
    virtual void onKeyEvent(int key, int scancode, int action, int mods) = 0;
};
