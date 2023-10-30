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

void 
minecraft(std::array<char, N_VOXELS> &voxels, glm::ivec2 cornerXY) {
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    std::array<std::array<int, SIZE_Y>, SIZE_X> elevation;

    // Compute elevation
    for(int x = 0; x < SIZE_X; x++) {
        for(int y = 0; y < SIZE_Y; y++) {
            // World voxel position from chunk position 
            int xx = x + cornerXY.x;
            int yy = y + cornerXY.y;

            elevation[x][y] = SIZE_Z * (1.0f + noise.GetNoise((float) xx, (float) yy))/2.0f;
        }
    }

    // Populate voxel data
    for(int i = 0; i < N_VOXELS; i++) {
        auto c = coord(i);

        if(c.z <= elevation[c.x][c.y])
            voxels[i] = 0x01;
        else
            voxels[i] = 0x00;
    }
}
