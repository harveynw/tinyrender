#pragma once

#include "glm/ext.hpp"

#include "Camera.hpp"
#include "../webgpu/primitives/buffers/uniforms/ViewProjMatrixUniform.hpp"


using glm::vec2;

class TurntableCamera : public Camera {
protected:
    // Current view
    float phi = 0.8f;
    float theta = 0.5f;
    float r = 1.2f;

    // Mouse State
    bool dragging = false;
    vec2 startPosition;

    // Config
    float mouseSensitivity = 0.01f;
    float scrollSensitivity = 0.1f;

    void updateInternalBuffer() override;
public:
    TurntableCamera() = default;

    glm::vec3 getPosition() override;

    void onFrame(float dt) override;
    void enableListen(GLFWwindow *window, std::shared_ptr<engine::ViewProjMatrixUniform> vpMatrix) override;

    void onMouseMove(double xpos, double ypos) override;
    void onMouseButton(GLFWwindow *window, int button, int action, int mods) override;
    void onScroll(double xoffset, double yoffset) override;
    void onKeyEvent(int key, int scancode, int action, int mods) override;
};

