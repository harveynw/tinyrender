#pragma once

#ifndef __EMSCRIPTEN__

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

/*
*   OpenCV implementation of "Fourier Synthesis of Ocean Scenes" (1986, Sandia National Laboratories)
*/

class FFTopencv {
protected:
    int DISPLACE_MAP_RES;

    cv::Mat whiteNoise;
    cv::Mat filter;
    cv::Mat displace;

    cv::Mat constructFilter(cv::Size2i size);
    void computeDft(cv::Mat &src, cv::Mat &dst);
    void fftshift(const cv::Mat &input_img, cv::Mat &output_img);
    void applyFilter(cv::Mat &src, cv::Mat &filter, cv::Mat &dst);
    float PMSpectrum2D(float f, float theta);

    void phaseShiftRoutine(cv::Mat &src, cv::Mat &dst, float dt);
public:
    FFTopencv(int);
    void update(float dt, std::vector<uint8_t> &displacementData);
};

#endif