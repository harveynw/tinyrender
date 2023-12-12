#include "CameraImpl.hpp"

tinyrender::Camera::Camera(std::unique_ptr<CameraImpl> c) {
    this->camera = std::move(c);
}

tinyrender::Camera::~Camera() = default;

glm::vec3 
tinyrender::Camera::getPosition() {
    return this->camera->getPosition();
}