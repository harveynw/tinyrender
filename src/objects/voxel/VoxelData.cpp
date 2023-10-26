#include "VoxelData.hpp"

void 
sphere(std::array<char, N_VOXELS> &voxels, int radius) {
    for(int i = 0; i < N_VOXELS; i++) {
        auto c = coord(i);
        
        // Air
        voxels[i] = 0x00;

        // Sphere around centre
        auto cc = glm::vec3(c) - glm::vec3(SIZE_X/2.0, SIZE_Y/2.0, SIZE_Z/2.0);
        float r = glm::sqrt(glm::dot(cc, cc));
        if(r < radius) {
            voxels[i] = 0x01;
        }
    }
}

void 
fill(std::array<char, N_VOXELS> &voxels) {
    for(int i = 0; i < N_VOXELS; i++) 
        voxels[i] = 0x01;
}
