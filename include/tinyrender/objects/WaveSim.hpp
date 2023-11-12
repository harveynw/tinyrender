#pragma once

#include <cmath>
#include <vector>

#include "Object.hpp"

namespace tinyrender {

    class WaveSim : public Object {
    private:
        using Object::setTexture; // disable
    protected:
        // Properties
        float WIDTH; // World space width
        float LENGTH; // World space height
        int MESH_RES_WIDTH = 256; // Mesh resolution
        int MESH_RES_LENGTH = 256; // Mesh resolution
        int DISP_MAP_RES = 512; // Resolution of the displacement map (square)

        // Grid mesh for drawing the waves
        std::shared_ptr<tinyrender::AttributeBuffer> mesh = nullptr;

        // Displacement of the surface is stored in a 1D vector, and uploaded to a texture on the GPU
        std::vector<uint8_t> displacementData;
        std::shared_ptr<tinyrender::Texture2D::WavesDataTexture> texture;

        // Simulation methods
        std::unique_ptr<NaiveWaveSim> naive;
        std::unique_ptr<FFTfftw> fftw;
    public:
        WaveSim(float width, float length);

        void onInit(Context *c, Scene *s) override;
        void onUpdate(State &state, float dt) override;
        void onRemove() override;

        void setColor(glm::vec3 c) override;

        ~WaveSim();
    };

}