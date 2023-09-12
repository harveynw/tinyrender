//
// Created by Harvey Williams on 13/08/2023.
//

#ifndef TINYGAME_TURNTABLE_H
#define TINYGAME_TURNTABLE_H

#include "glm/ext.hpp"

#include "Controller.hpp"
#include "../webgpu/primitives/buffers/uniforms/UniformViewProjection.hpp"


using glm::vec2;

class TurntableController : public Controller {
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
    explicit TurntableController(std::shared_ptr<engine::UniformViewProjection> uniforms);

    void tick() override {};
    void enableListen(GLFWwindow *window) override { (void) window; };

    void onMouseMove(double xpos, double ypos) override;
    void onMouseButton(GLFWwindow *window, int button, int action, int mods) override;
    void onScroll(double xoffset, double yoffset) override;
    void onKeyEvent(int key, int scancode, int action, int mods) override;
};


#endif //TINYGAME_TURNTABLE_H
