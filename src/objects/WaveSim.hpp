#pragma once

#include <cmath>

#include "Object.hpp"
#include "loaders/Polygons.hpp"
#include "loaders/Shapes.hpp"
#include "../webgpu/primitives/textures/WavesDataTexture.hpp"

#ifndef __EMSCRIPTEN__
#include "wavesim/FFTopencv.hpp"
#endif
#include "wavesim/naive.hpp"
#include "wavesim/FFTfftw.hpp"


namespace engine {

    class WaveSim : public Object {
    private:
        using Object::setTexture; // disable
    protected:
        Context *context;
        Scene *scene;

        int MESH_RES_WIDTH = 256;
        int MESH_RES_LENGTH = 256;

        int DISP_MAP_RES = 512;

        std::vector<uint8_t> displacementData;
        std::shared_ptr<engine::Texture2D::WavesDataTexture> texture;

        #ifndef __EMSCRIPTEN__
        std::unique_ptr<FFTopencv> openCVSim;
        #endif
        std::unique_ptr<NaiveWaveSim> naive;
        std::unique_ptr<FFTfftw> fftw;
    public:
        WaveSim(Context *c, Scene *s, float width, float length);
        void update(float dt) override;
        void setColor(glm::vec3 c) override;
    };

}