#pragma once

#include "glm/ext.hpp"

#include "Camera.hpp"
#include "../webgpu/primitives/buffers/uniforms/ViewProjMatrixUniform.hpp"


using glm::vec3;
using glm::vec2;


class FreeviewCamera : public Camera {
protected:
    GLFWwindow *glfw_window;

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
    bool captureMouse = false;
    bool fast = false;

    // Config
    float moveSpeed = 10.0f;
    float fastSpeed = 50.0f;
    float mouseSensitivity = 0.01f;

    void updateInternalBuffer() override;
    void updateMouseState();
public:
    FreeviewCamera() = default;

    void onFrame(float dt) override;
    void enableListen(GLFWwindow *window, std::shared_ptr<engine::ViewProjMatrixUniform> vpMatrix) override;

    void onMouseMove(double xpos, double ypos) override;
    void onMouseButton(GLFWwindow *window, int button, int action, int mods) override;
    void onScroll(double xoffset, double yoffset) override;
    void onKeyEvent(int key, int scancode, int action, int mods) override;
};

