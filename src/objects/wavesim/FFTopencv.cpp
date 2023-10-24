#ifndef __EMSCRIPTEN__

#include "FFTopencv.hpp"


FFTopencv::FFTopencv(int displacementMapRes): DISPLACE_MAP_RES(displacementMapRes) {
    whiteNoise = cv::Mat::eye(displacementMapRes, displacementMapRes, CV_8U);
    cv::randu(whiteNoise, 0, 255);

    filter = constructFilter(whiteNoise.size());

    // Go float
    cv::Mat fImage;
    whiteNoise.convertTo(fImage, CV_32F);
    cv::normalize(fImage, fImage, 0, 1, cv::NORM_MINMAX);

    cv::Mat dft_output;
    computeDft(fImage, dft_output);
    applyFilter(dft_output, this->filter, dft_output);
    cv::dft(dft_output, displace, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);

    // Scale correctly back to uint8
    cv::normalize(displace, displace, 0, 1, cv::NORM_MINMAX);
    displace = 255 * displace;
    displace.convertTo(displace, CV_8U);
    displace = displace.clone(); // Forces the texture to be contiguous 

    //cv::imshow("Output image", img_out);
    //cv::waitKey(0);
    //exit(0);
}

void
FFTopencv::update(float dt, std::vector<uint8_t> &displacementData) {
    (void) dt;

    // Adhoc method, shift frequencies
    cv::Mat fImage;
    displace.convertTo(fImage, CV_32F);
    cv::normalize(fImage, fImage, 0, 1, cv::NORM_MINMAX);

    cv::Mat dft_output;
    computeDft(fImage, dft_output);

    phaseShiftRoutine(dft_output, dft_output, dt); // Perform shift

    cv::dft(dft_output, displace, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);
    cv::normalize(displace, displace, 0, 1, cv::NORM_MINMAX); // TODO: Potential bug, we are destroying information
    displace = 255 * displace;
    displace.convertTo(displace, CV_8U);
    displace = displace.clone(); // Forces the texture to be contiguous 

    // Bodge update displacement
    auto texel = (uint8_t*) displace.data;
    for (int i = 0; i < DISPLACE_MAP_RES; ++i) {
        for (int j = 0; j < DISPLACE_MAP_RES; ++j) {
            uint8_t *p = &displacementData[4 * (j * DISPLACE_MAP_RES + i)];
            p[0] = *texel;
            texel++;
        }
    }
}

cv::Mat
FFTopencv::constructFilter(cv::Size2i size) {
    cv::Mat H(size, CV_32F, cv::Scalar(1));
    auto pi = glm::pi<float>();

    for(int u = 0; u < size.height; u++) {
        for(int v = 0; v < size.width; v++) {
            // Center
            float uu = u-size.height/2.0;
            float vv = v-size.width/2.0;

            // Compute spectrum
            float f = cv::sqrt(uu*uu + vv*vv)/size.width;

            // Compute argument
            float angle = atan2(uu, vv); // [-pi, pi]
            if(angle < -pi/2.0)
                angle = pi + angle;
            else if(angle > pi/2.0)
                angle = pi - angle;

            // 2D PM Spectrum filter
            if(f != 0.0)
                H.at<float>(u, v) = PMSpectrum2D(f, angle);
        }
    }

    cv::normalize(H, H, 0, 1, cv::NORM_MINMAX);
    fftshift(H, H);
    return H;
}

void
FFTopencv::computeDft(cv::Mat &src, cv::Mat &dst) {
    cv::Mat planes[] = { src, cv::Mat::zeros(src.size(), CV_32F)};
    cv::Mat complex;
    cv::merge(planes, 2, complex);

    // Complex is a { Re, 0 } mat, perform DFT in place to get { Re, Img }
    dft(complex, complex);
    dst = complex;
}

void
FFTopencv::fftshift(const cv::Mat &input_img, cv::Mat &output_img)
{
    output_img = input_img.clone();
    int cx = output_img.cols / 2;
    int cy = output_img.rows / 2;
    cv::Mat q1(output_img, cv::Rect(0, 0, cx, cy));
    cv::Mat q2(output_img, cv::Rect(cx, 0, cx, cy));
    cv::Mat q3(output_img, cv::Rect(0, cy, cx, cy));
    cv::Mat q4(output_img, cv::Rect(cx, cy, cx, cy));

    cv::Mat temp;
    q1.copyTo(temp);
    q4.copyTo(q1);
    temp.copyTo(q4);
    q2.copyTo(temp);
    q3.copyTo(q2);
    temp.copyTo(q3);
}

void
FFTopencv::applyFilter(cv::Mat &src, cv::Mat &filter, cv::Mat &dst) {
    cv::Mat dft[] = { cv::Mat::zeros(src.size(), CV_32F), cv::Mat::zeros(src.size(), CV_32F)};
    cv::split(src, dft);

    // From complex frequency domain to magnitude-phase
    cv::Mat mag; cv::Mat phase;
    cv::cartToPolar(dft[0], dft[1], mag, phase);

    // Multiply magnitude by filter only, phase remains unchanged
    mag = filter.mul(mag);

    // Back to complex frequency domain
    cv::polarToCart(mag, phase, dft[0], dft[1]);

    // Output
    cv::merge(dft, 2, dst);
}

float
FFTopencv::PMSpectrum2D(float f, float theta) {
    auto pi = glm::pi<float>();

    float f_m = 0.13 * 9.81 / 15;

    // Direction in 2D
    float p = 9.77 * pow(f/f_m, f < f_m ? 4.06 : -2.34);
    float N_p = pow(2.0, 1-2*p) * pi * tgamma(2*p +1) / pow(tgamma(p+1), 2.0);
    float D = 1.0 / N_p * pow(cos(theta/2.0), 2*p);

    // PM spectrum
    float term_1 = (0.0081 * pow(9.81, 2.0)) / (pow(2*pi, 4)*pow(f, 5));
    float term_2 = exp(-5.0/4.0 * pow(f_m / f, 4));
    float F_pm = term_1 * term_2;

    return D * F_pm;
}

void 
FFTopencv::phaseShiftRoutine(cv::Mat &src, cv::Mat &dst, float dt) {
    cv::Mat dft[] = { cv::Mat::zeros(src.size(), CV_32F), cv::Mat::zeros(src.size(), CV_32F)};
    cv::split(src, dft);

    double max;
    cv::minMaxIdx(dft[0], nullptr, &max);

    cv::Mat mag, phase; 
    cv::cartToPolar(dft[0], dft[1], mag, phase);

    /* From Fourier Ocean Paper, doesn't work
    auto size = dft[0].size();
    (void) dt;
    for(int u = 0; u < size.height; u++) {
        for(int v = 0; v < size.width; v++) {
            float freq = dft[0].at<float>(u, v);
            if(freq > 2/3 * max)
                phase.at<float>(u, v) += 0.21; //0.05;
            else
                phase.at<float>(u, v) += 0.15; //0.03;
        }
    }
    */

    // Simple translation
    phase = phase + 4.0 * dt * cv::Mat::ones(src.size(), CV_32F);

    cv::polarToCart(mag, phase, dft[0], dft[1]);
    cv::merge(dft, 2, dst);
}

#endif
