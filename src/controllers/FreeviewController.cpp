//
// Created by Harvey Williams on 15/08/2023.
//

#include "FreeviewController.hpp"

FreeviewController::FreeviewController(std::shared_ptr<engine::UniformViewProjection> uniforms) {
    this->uniforms = uniforms;
}

void
FreeviewController::updateInternalBuffer() {
    this->uniforms->updateViewMatrix(glm::lookAt(position, position + direction,
                                                 vec3(0, 0, 1)));
}

void
FreeviewController::onMouseMove(double xpos, double ypos) {
    vec2 current = vec2((float) -xpos, (float) ypos);
    vec2 delta = mousePosition - current;

    phi -= delta.x * mouseSensitivity;
    theta -= delta.y * mouseSensitivity;
    theta = glm::clamp(theta, 1e-5f, glm::pi<float>() - 1e-5f);

    mousePosition = current;

    direction = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));

    updateInternalBuffer();
}

void
FreeviewController::onKeyEvent(int key, int scancode, int action, int mods) {
    (void) scancode; (void) mods;
    if(key == GLFW_KEY_W)
        longitudinal = action == GLFW_PRESS ? 1 : (action == GLFW_RELEASE ? 0 : longitudinal);
    if(key == GLFW_KEY_S)
        longitudinal = action == GLFW_PRESS ? -1 : (action == GLFW_RELEASE ? 0 : longitudinal);
    if(key == GLFW_KEY_A)
        lateral = action == GLFW_PRESS ? 1 : (action == GLFW_RELEASE ? 0 : lateral);
    if(key == GLFW_KEY_D)
        lateral = action == GLFW_PRESS ? -1 : (action == GLFW_RELEASE ? 0 : lateral);
    if(key == GLFW_KEY_SPACE)
        vertical = action == GLFW_PRESS ? 1 : (action == GLFW_RELEASE ? 0 : vertical);
    if(key == GLFW_KEY_Z)
        vertical = action == GLFW_PRESS ? -1 : (action == GLFW_RELEASE ? 0 : vertical);
}

void FreeviewController::tick() {
    if(longitudinal == 0 && lateral == 0 && vertical == 0)
        return; // Not moving

    if(lateral != 0) {
        // This is a little wasteful recomputing the rotation matrices every tick but simple
        float angle = (float) -lateral * glm::pi<float>()/2.0f;
        glm::mat2x2 rot = glm::mat2x2(cos(angle), -sin(angle), sin(angle),
                                      cos(angle));
        glm::vec2 rotDir = rot * glm::vec2(direction.x, direction.y);
        position.x += moveSpeed * rotDir.x;
        position.y += moveSpeed * rotDir.y;
    }

    if(longitudinal != 0)
        position += (float) longitudinal * moveSpeed * direction;

    if(vertical != 0)
        position += (float) vertical * moveSpeed * vec3(0, 0, 1);

    updateInternalBuffer();
}

void
FreeviewController::onMouseButton(GLFWwindow *window, int button, int action, int mods) {
    (void) window;
    (void) button;
    (void) action;
    (void) mods;
}

void
FreeviewController::onScroll(double xoffset, double yoffset) {
    (void) xoffset;
    (void) yoffset;
}

void FreeviewController::enableListen(GLFWwindow *window) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
