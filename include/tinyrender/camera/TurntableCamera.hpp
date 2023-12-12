#pragma once

#include "Camera.hpp"

namespace tinyrender {

    class TurntableCamera : public tinyrender::Camera {
    public:
        TurntableCamera();
        ~TurntableCamera();
    };

}