#pragma once

#include <memory>
#include <glm/glm.hpp>

class CameraImpl;
class EngineImpl;

namespace tinyrender {

    class Camera {
    protected:
        std::unique_ptr<CameraImpl> camera;
        friend EngineImpl;
    public:
        explicit Camera(std::unique_ptr<CameraImpl>);
        virtual ~Camera();

        glm::vec3 getPosition();
    };

}