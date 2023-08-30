//
// Created by Harvey Williams on 12/08/2023.
//

#ifndef TINYGAME_DEBUGTEXTURE2D_H
#define TINYGAME_DEBUGTEXTURE2D_H

#include "Texture2D.hpp"

namespace engine {

    class DebugTexture2D : public Texture2D {
    protected:
        std::vector<uint8_t> data;
        void generateData();
    public:
        DebugTexture2D(Engine *engine, uint32_t width, uint32_t height);
        ~DebugTexture2D() = default;
    };

}

#endif //TINYGAME_DEBUGTEXTURE2D_H
