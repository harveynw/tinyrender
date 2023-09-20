#include "mips.hpp"

void
downsizeImage(std::vector<uint8_t> &img, int width, int height, std::vector<uint8_t> &dest) {
    // 2x2 averaging convolution with stride of 2

    if((int) dest.size() != 4 * width * height)
        throw std::invalid_argument("Destination vector does not match dimensions passed.");
    if((int) img.size() != 4 * (2*width) * (2*height))
        throw std::invalid_argument("Destination image must be half the width and height of the input image.");

    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            auto p00 = img.data() + 4 * ((2 * j) * (2 * width) + (2 * i));
            auto p01= img.data() + 4 * ((2 * j) * (2 * width + (2 * i + 1)));
            auto p10 = img.data() + 4 * ((2 * j + 1) * (2 * width) + (2 * i));
            auto p11 = img.data() + 4 * ((2 * j + 1) * (2 * width) + (2 * i + 1));
            // Average
            auto p = &dest[4 * (j * width + i)];
            p[0] = (*p00 + *p01 + *p10 + *p11) / 4;
            p[1] = (*(p00+1) + *(p01+1) + *(p10+1) + *(p11+1)) / 4;
            p[2] = (*(p00+2) + *(p01+2) + *(p10+2) + *(p11+2)) / 4;
            p[3] = (*(p00+3) + *(p01+3) + *(p10+3) + *(p11+3)) / 4;
        }
    }
}
