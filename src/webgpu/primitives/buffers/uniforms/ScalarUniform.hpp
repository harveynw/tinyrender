#pragma once

#include "../Buffer.hpp"

namespace tinyrender {

    class ScalarUniform : public Buffer {
    protected:
        ScalarUniforms uniform{};
    public:
        explicit ScalarUniform(Context *context);
        void set(float value);
    };

}
