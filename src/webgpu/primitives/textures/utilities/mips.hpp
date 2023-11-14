#pragma once

#include <vector>
#include <inttypes.h>
#include <stdexcept>

void downsizeImage(std::vector<uint8_t> &img, int width, int height, std::vector<uint8_t> &dest);
