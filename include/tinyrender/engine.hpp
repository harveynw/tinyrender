#pragma once

#include <memory>

#include "camera/Camera.hpp"
#include "objects/Object.hpp"

class EngineImpl;

namespace tinyrender {
    class Engine {
    protected:
        std::unique_ptr<EngineImpl> engine;
    public:
        Engine();
        ~Engine();

        // Creates the window
        void launch(int width, int height); 

        // Is rendering engine running or not
        bool isRunning();

        // Draw frame method
        void onFrame();

        // Add object to the scene
        void addObject(std::shared_ptr<tinyrender::Object>);

        // Set the camera
        void setCamera(std::shared_ptr<tinyrender::Camera>);
    };
}