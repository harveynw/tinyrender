#pragma once

#include "glm/ext.hpp"

#include "CameraImpl.hpp"
#include "../webgpu/primitives/uniforms/ViewProjMatrixUniform.hpp"
#include "camera/TurntableCamera.hpp"

using glm::vec2;

class TurntableCameraImpl : public CameraImpl {
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
    TurntableCameraImpl() = default;

    glm::vec3 getPosition() override;

    void onFrame(float dt) override;
    void enableListen(GLFWwindow *window, std::shared_ptr<ViewProjMatrixUniform> vpMatrix) override;

    void onMouseMove(double xpos, double ypos) override;
    void onMouseButton(GLFWwindow *window, int button, int action, int mods) override;
    void onScroll(double xoffset, double yoffset) override;
    void onKeyEvent(int key, int scancode, int action, int mods) override;
};

