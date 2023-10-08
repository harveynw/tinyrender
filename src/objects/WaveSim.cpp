#include "WaveSim.hpp"

engine::WaveSim::WaveSim(Context *c, Scene *s, float width, float length) {
    this->context = c;
    this->scene = s;
    this->targetPipeline = Waves;

    assert(MESH_RES_WIDTH % 2 == 0);
    assert(MESH_RES_LENGTH % 2 == 0);

    this->displacementData = std::vector<uint8_t>(4 * DISP_MAP_RES * DISP_MAP_RES);

    Polygons p;
    loadPlaneMesh(p, width, length, MESH_RES_WIDTH, MESH_RES_LENGTH);

    auto attrs = std::make_shared<engine::AttributeBuffer>(this->context, p.data, p.vertices);
    this->resources = std::make_shared<ObjectResources>(this->context, this->scene, attrs, ColoredTriangle);

    // Init Waves Data and reset Bind Group to point to it
    this->texture = std::make_shared<engine::Texture2D::WavesDataTexture>(c, DISP_MAP_RES, DISP_MAP_RES);
    this->resources->texture = this->texture;
    this->resources->maxDisplacement = std::make_shared<engine::ScalarUniform>(c);
    this->resources->maxDisplacement->set(0.2);
    this->resources->resetBindGroup(c, s, Waves);

    #ifndef __EMSCRIPTEN__
    whiteNoise = cv::Mat::eye(DISP_MAP_RES, DISP_MAP_RES, CV_8U);
    cv::randu(whiteNoise, 0, 255);
    #endif
}

void
engine::WaveSim::update(float dt) {
    naiveWaveSim(dt);
    fourierSynthesis(dt);
}

void
engine::WaveSim::setColor(glm::vec3 c) {
    this->resources->color->set(c);
}

void
engine::WaveSim::naiveWaveSim(float dt) {
    /*
     * Naive wave sim
     */
    this->time += 0.05f * dt;

    float waves_s = 1.0;
    float waves_c = 1.0;
    float time_speed = waves_s * 100;

    for (int i = 0; i < DISP_MAP_RES; ++i) {
        for (int j = 0; j < DISP_MAP_RES; ++j) {
            uint8_t *p = &displacementData[4 * (j * DISP_MAP_RES + i)];
            // Only store displacement in the first channel
            float var = (glm::sin(waves_s * (i + time_speed * this->time)) + glm::cos(waves_c * (j + time_speed * this->time)));
            p[0] = (uint8_t) (255 * (var + 2.0)/4.0);
        }
    }

    this->texture->update(displacementData);
}


#ifndef __EMSCRIPTEN__
void
engine::WaveSim::fourierSynthesis(float dt) {
    (void) dt;

    // Moved to init
    //cv::Mat whiteNoise = cv::Mat::eye(DISP_MAP_RES, DISP_MAP_RES, CV_8U);
    //cv::randu(whiteNoise, 0, 255);

    //cv::imshow("White Noise", whiteNoise);
    //cv::waitKey(0); // Wait for a keystroke in the window

    // Go float
    cv::Mat fImage;
    whiteNoise.convertTo(fImage, CV_32F);

    cv::Mat dft_output;
    computeDft(fImage, dft_output);

    cv::Mat filter = constructFilter(fImage.size());
    applyFilter(dft_output, filter, dft_output);

    cv::Mat img_out;
    cv::dft(dft_output, img_out, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);

    //img scale attempt
    cv::normalize(img_out, img_out, 0, 1, cv::NORM_MINMAX);
    img_out = 255 * img_out;

    img_out.convertTo(img_out, CV_8U);
    //cv::imshow("Output image of filtering", img_out);
    //cv::waitKey(0);
    //exit(0);

    // Bodge update displacement
    img_out = img_out.clone(); // contigious
    auto texel = (uint8_t*) img_out.data;
    for (int i = 0; i < DISP_MAP_RES; ++i) {
        for (int j = 0; j < DISP_MAP_RES; ++j) {
            uint8_t *p = &displacementData[4 * (j * DISP_MAP_RES + i)];
            p[0] = *texel;
            texel++;
        }
    }
    this->texture->update(displacementData);
}

cv::Mat engine::WaveSim::constructFilter(cv::Size2i size) {
    cv::Mat H(size, CV_32F, cv::Scalar(1));
    auto pi = glm::pi<float>();

    for(int u = 0; u < size.height; u++) {
        for(int v = 0; v < size.width; v++) {
            // Center
            float uu = u-size.height/2.0;
            float vv = v-size.width/2.0;

            // Compute spectrum
            float f = cv::sqrt(uu*uu + vv*vv)/size.width;

            // PM 2D filter
            float angle = atan2(uu, vv); // [-pi, pi]
            if(angle < -pi/2.0)
                angle = pi + angle;
            else if(angle > pi/2.0)
                angle = pi - angle;

            if(f != 0.0)
                H.at<float>(u, v) = PMSpectrum2D(f, angle);

            // Gaussian filter
            //H.at<float>(u, v) = f < 0.1 ? 0.0 : exp(-pow(f, 2)/(0.8 * pow(0.30, 2)));
        }
    }

    cv::normalize(H, H, 0, 1, cv::NORM_MINMAX);
    return H;
}

void
engine::WaveSim::computeDft(cv::Mat &src, cv::Mat &dst) {
    cv::Mat planes[] = { src, cv::Mat::zeros(src.size(), CV_32F)};
    cv::Mat complex;
    cv::merge(planes, 2, complex);

    // Complex is a { Re, 0 } mat, perform DFT in place to get { Re, Img }
    dft(complex, complex);
    dst = complex;
}

void engine::WaveSim::fftshift(const cv::Mat &input_img, cv::Mat &output_img)
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
engine::WaveSim::applyFilter(cv::Mat &src, cv::Mat &filter, cv::Mat &dst) {
    cv::Mat dft[] = { cv::Mat::zeros(src.size(), CV_32F), cv::Mat::zeros(src.size(), CV_32F)};
    cv::split(src, dft);

    fftshift(dft[0], dft[0]);

    // From complex frequency domain to magnitude-phase
    cv::Mat mag; cv::Mat phase;
    cv::cartToPolar(dft[0], dft[1], mag, phase);

    float sigpwr = mag.dot(mag); // sum of squares of elements

    // Debugging
    //cv::imshow("Frequency domain", dft[0]);
    //cv::waitKey(0);
    //cv::imshow("The filter", filter);
    //cv::waitKey(0);
    //cv::imshow("FFT magnitude of input", mag);
    //cv::waitKey(0);

    // Multiply magnitude by filter only, phase remains unchanged
    mag = filter.mul(mag);

    // Filter normalization
    float sigpwr2 = mag.dot(mag);
    mag = mag * cv::sqrt(sigpwr/sigpwr2);

    // Back to complex frequency domain
    cv::polarToCart(mag, phase, dft[0], dft[1]);
    fftshift(dft[0], dft[0]);

    // More debugging
    //cv::imshow("Frequency domain filtered", dft[0]);
    //cv::waitKey(0);

    // Output
    cv::merge(dft, 2, dst);
}

float
engine::WaveSim::PMSpectrum2D(float f, float theta) {
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
#endif
