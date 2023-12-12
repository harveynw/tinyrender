#pragma once

#include "Object.hpp"

class WaveSimImpl;

namespace tinyrender {

    class WaveSim : public Object {
    public:
        WaveSim(float width, float length);
        ~WaveSim();
    };

}