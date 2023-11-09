//
// Created by Harvey Williams on 15/08/2023.
//

#include "FreeviewCamera.hpp"

void
FreeviewCamera::updateInternalBuffer() {
    this->viewProjectionMatrix->updateCameraPosition(position);
    this->viewProjectionMatrix->updateViewMatrix(glm::lookAt(position, position + direction,
                                                 vec3(0, 0, 1)));
}

void
FreeviewCamera::onMouseMove(double xpos, double ypos) {
    if(!captureMouse)
        return;

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
FreeviewCamera::onKeyEvent(int key, int scancode, int action, int mods) {
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
    if(key == GLFW_KEY_E)
        fast = action == GLFW_PRESS ? true : (action == GLFW_RELEASE ? false : fast);
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        captureMouse = !captureMouse;
        updateMouseState();
    }
}

glm::vec3 
FreeviewCamera::getPosition() {
    return this->position;
}

void FreeviewCamera::onFrame(float dt)
{
    float speed = fast ? fastSpeed : moveSpeed;

    if(longitudinal == 0 && lateral == 0 && vertical == 0)
        return; // Not moving

    if(lateral != 0) {
        float angle = (float) -lateral * glm::pi<float>()/2.0f;
        glm::mat2x2 rot = glm::mat2x2(cos(angle), -sin(angle), sin(angle),
                                      cos(angle));
        glm::vec2 rotDir = rot * glm::vec2(direction.x, direction.y);
        position.x += dt * speed * rotDir.x;
        position.y += dt * speed * rotDir.y;
    }

    if(longitudinal != 0)
        position += (float) longitudinal * dt * speed * direction;

    if(vertical != 0)
        position += (float) vertical * dt * speed * vec3(0, 0, 1);

    updateInternalBuffer();
}

void
FreeviewCamera::onMouseButton(GLFWwindow *window, int button, int action, int mods) {
    (void) window;
    (void) mods;

    // Capture if not captured and left mouse press
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        captureMouse = true;
        updateMouseState();

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        mousePosition = vec2(-xpos, ypos);
    }
}

void
FreeviewCamera::onScroll(double xoffset, double yoffset) {
    (void) xoffset;
    (void) yoffset;
}

void FreeviewCamera::updateMouseState() {
    glfwSetInputMode(glfw_window, GLFW_CURSOR, captureMouse ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void FreeviewCamera::enableListen(GLFWwindow *window, std::shared_ptr<tinyrender::ViewProjMatrixUniform> vpMatrix) {
    this->viewProjectionMatrix = vpMatrix;
    printf("Freeview Camera enabled - WASD, Space - Up, Z - Down, E - Speed, Esc - Uncapture\n");
    glfw_window = window;
    updateInternalBuffer();
    updateMouseState();
}
