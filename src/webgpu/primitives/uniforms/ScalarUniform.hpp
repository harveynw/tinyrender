#pragma once

#include "BufferUniform.hpp"

namespace tinyrender {

    class ScalarUniform : public BufferUniform {
    protected:
        ScalarUniforms uniform{};
    public:
        explicit ScalarUniform(Context *context);
        void set(float value);
    };

}
