#pragma once

#include <vector>
#include <fftw3.h>
#include <random>
#include <glm/ext.hpp>

/*
*   FFTW implementation of "Fourier Synthesis of Ocean Scenes" (1986, Sandia National Laboratories)
*/

class FFTfftw {
protected:
    int DISPLACE_MAP_RES;

    fftw_complex *filter;
    fftw_complex *fft;
    fftw_complex *displace_complex;
    double *displace;

    float time = 0.0;

    void initDisplace();
    void initFilter();

public:
    FFTfftw(int);
    ~FFTfftw();
    void update(float dt, std::vector<uint8_t> &displacementData);
};

void re(int N, fftw_complex *src, double *dst);
void print(int N, fftw_complex *src);
void print(int N, double *src);
void clamp(int N, double a, double b, fftw_complex *src, fftw_complex *dst);
void scale(int N, double c, fftw_complex *src, fftw_complex *dst);
void scale(int N, double c, double *src, double *dst);
void abs(int N, double *src, double *dst);
void normalizeIFFT(int N, double *src, double *dst);
void normalize(int N, fftw_complex *src, fftw_complex *dst);
void normalize(int N, double *src, double *dst);
void cartToPolar(int N, fftw_complex *src, fftw_complex *dst);
void polarToCart(int N, fftw_complex *src, fftw_complex *dst);
void shiftCoord(int n, int i, int j, int &ii, int &jj);
double signalPower(int N, fftw_complex *src);
float PMSpectrum2D(float f, float theta);

void slow_fft_phase_shift(int n, double *src, double *dst);
void slow_fft_forward(int n, double *src, fftw_complex *dst);
void slow_fft_forward(int n, fftw_complex *src, fftw_complex *dst);
void slow_fft_backward(int n, fftw_complex *src, double *dst);
void slow_fft_backward(int n, fftw_complex *src, fftw_complex *dst);
