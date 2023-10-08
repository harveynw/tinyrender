#pragma once

#ifndef __EMSCRIPTEN__
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#endif

#include <cmath>

#include "Object.hpp"
#include "loaders/Polygons.hpp"
#include "loaders/Shapes.hpp"
#include "../webgpu/primitives/textures/WavesDataTexture.hpp"

namespace engine {

    class WaveSim : public Object {
    private:
        using Object::setTexture; // disable
    protected:
        Context *context;
        Scene *scene;

        int MESH_RES_WIDTH = 128;
        int MESH_RES_LENGTH = 128;

        int DISP_MAP_RES = 512;

        float time = 0.0;
        std::vector<uint8_t> displacementData;
        std::shared_ptr<engine::Texture2D::WavesDataTexture> texture;

        void naiveWaveSim(float dt);

        #ifndef __EMSCRIPTEN__
        // Fourier Wave Functions
        cv::Mat whiteNoise;
        void fourierSynthesis(float dt);
        cv::Mat constructFilter(cv::Size2i size);
        void computeDft(cv::Mat &src, cv::Mat &dst);
        void fftshift(const cv::Mat &input_img, cv::Mat &output_img);
        void applyFilter(cv::Mat &src, cv::Mat &filter, cv::Mat &dst);
        float PMSpectrum2D(float f, float theta);
        #endif
    public:
        WaveSim(Context *c, Scene *s, float width, float length);
        void update(float dt) override;
        void setColor(glm::vec3 c) override;
    };

}