//
// Created by Harvey Williams on 13/08/2023.
//

#pragma once

#include "glm/ext.hpp"

#include "Controller.hpp"
#include "../webgpu/primitives/buffers/uniforms/UniformViewProjection.hpp"


using glm::vec3;
using glm::vec2;


class FreeviewController : public Controller {
protected:
    // Current view
    float phi = 0.0f;
    float theta = glm::pi<float>()/2.0;
    vec3 position = vec3(0, 0, 1);
    vec3 direction = vec3(1, 0, 0); // start facing positive x direction

    // Keyboard State
    int lateral = 0;
    int longitudinal = 0;
    int vertical = 0;
    vec2 mousePosition;

    // Config
    float moveSpeed = 0.1f;
    float mouseSensitivity = 0.01f;

    void updateInternalBuffer() override;
public:
    explicit FreeviewController(std::shared_ptr<engine::UniformViewProjection> uniforms);

    void tick() override;
    void enableListen(GLFWwindow *window) override;

    void onMouseMove(double xpos, double ypos) override;
    void onMouseButton(GLFWwindow *window, int button, int action, int mods) override;
    void onScroll(double xoffset, double yoffset) override;
    void onKeyEvent(int key, int scancode, int action, int mods) override;
};

