//
// Created by Harvey Williams on 13/08/2023.
//

#pragma once

#include <GLFW/glfw3.h>
#include "../webgpu/primitives/buffers/uniforms/ViewProjMatrixUniform.hpp"


class Controller {
protected:
    virtual void updateInternalBuffer() = 0;
public:
    std::shared_ptr<engine::ViewProjMatrixUniform> uniforms;

    Controller() = default;
    virtual ~Controller() = default;

    virtual void tick() = 0;

    virtual void enableListen(GLFWwindow *window) = 0;

    virtual void onMouseMove(double xpos, double ypos) = 0;
    virtual void onMouseButton(GLFWwindow *window, int button, int action, int mods) = 0;
    virtual void onScroll(double xoffset, double yoffset) = 0;
    virtual void onKeyEvent(int key, int scancode, int action, int mods) = 0;
};
