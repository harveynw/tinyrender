//
// Created by Harvey Williams on 12/08/2023.
//

#pragma once

#include <filesystem>

#include "Texture2D.hpp"
#include "stb_image.h"

namespace fs = std::filesystem;

namespace engine {

    class ImageTexture2D : public Texture2D {
    public:
        ImageTexture2D(Engine *engine, const fs::path& path);
        ~ImageTexture2D() = default;
    };

}
