#pragma once

#include "Camera.hpp"

namespace tinyrender {

    class FreeviewCamera : public tinyrender::Camera {
    public:
        FreeviewCamera();
        ~FreeviewCamera();
    };

}