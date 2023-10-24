#include "FFTfftw.hpp"


FFTfftw::FFTfftw(int displacementMapRes): DISPLACE_MAP_RES(displacementMapRes) {
    initFilter();
    initDisplace();
}

FFTfftw::~FFTfftw()
{
    fftw_free(displace);
    fftw_free(displace_complex);
    fftw_free(fft);
    fftw_free(filter);
}

void
FFTfftw::update(float dt, std::vector<uint8_t> &displacementData) {
    this->time += dt;

    /*
    int n = DISPLACE_MAP_RES;
    int N = n * n;

    auto temp_displace_complex = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    auto temp_fft = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

    slow_fft_backward(n, fft, temp_displace_complex);
    slow_fft_forward(n, temp_displace_complex, temp_fft);

    cartToPolar(N, temp_fft, temp_fft);
    for(int i = 0; i < N; i++)
        temp_fft[i][1] += time;
    polarToCart(N, temp_fft, temp_fft);

    slow_fft_backward(n, temp_fft, displace_complex);
    fftw_free(temp_displace_complex);
    fftw_free(temp_fft);

    cartToPolar(N, displace_complex, displace_complex);
    re(N, displace_complex, displace);

    print(N, displace);
    normalize(N, displace, displace);
    
    re(N, disp, displace);
    normalizeIFFT(N, displace, displace);
    print(N, displace);
    normalize(N, displace, displace);

    */
    //slow_fft_phase_shift(DISPLACE_MAP_RES, displace, displace);

    for(int i = 0; i < DISPLACE_MAP_RES * DISPLACE_MAP_RES; i++) {
        displacementData[4*i] = (uint8_t) (255 * displace[i]);
        //displacementData[4*i] = (uint8_t) (255 * filter[i][0]);
    }
}

void FFTfftw::initDisplace()
{
    /*
    *   initDisplace(), allocates and populates "displace"
    */

    int n = DISPLACE_MAP_RES;
    int N = n * n;

    auto white_noise = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    displace = (double*) fftw_malloc(sizeof(double) * N);
    displace_complex = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    fft = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

    // Generate random noise in [0, 1]
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    for(int i = 0; i < N; i++) {
        white_noise[i][0] = dis(gen);
        white_noise[i][1] = 0.0;
    }

    slow_fft_forward(n, white_noise, fft);

    // Change to magnitude-phase
    cartToPolar(N, fft, fft);

    double pwr_1 = signalPower(N, fft);
    // Scale magnitude by filter
    for(int i = 0; i < N; i++)
        fft[i][0] *= filter[i][0];
    double pwr_2 = signalPower(N, fft);
    scale(N, sqrt(pwr_1/pwr_2), fft, fft);

    // Change back and then back to time domain
    polarToCart(N, fft, fft);

    slow_fft_backward(n, fft, displace_complex);
    re(N, displace_complex, displace);

    print(N, displace);
    normalize(N, displace, displace);

    fftw_free(white_noise);
}

void FFTfftw::initFilter()
{
    int N = DISPLACE_MAP_RES * DISPLACE_MAP_RES;
    int n = DISPLACE_MAP_RES;
    double pi = glm::pi<double>();

    filter = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {    
            int ii, jj;
            shiftCoord(n, i, j, ii, jj);

            // Center
            double u = (ii+0.5)-n/2.0;
            double v = (jj+0.5)-n/2.0;

            // Compute spectrum
            double f = sqrt(u*u + v*v)/n;
            double angle = atan2(u, v); 
            if(angle < -pi/2.0)
                angle = pi + angle;
            else if(angle > pi/2.0)
                angle = pi - angle;

            // Insert 2D PM Spectrum filter into the real component
            filter[i*n+j][0] = f != 0.0 ? PMSpectrum2D(f, angle) : 1.0;
        }
    }

    double sum_of_squares = 0.0;
    for(int i = 0; i < N; i++)
        //sum_of_squares += filter[i][0]*filter[i][0];
        sum_of_squares += abs(filter[i][0]);
    scale(N, 1/sqrt(sum_of_squares), filter, filter);
    normalize(N, filter, filter);
}

void 
re(int N, fftw_complex *src, double *dst)
{
    for(int i = 0; i < N; i++)
        dst[i] = src[i][0];
}

void print(int N, fftw_complex *src)
{
    for(int i = 0; i < N; i++)
        printf("(%f, %f), ", src[i][0], src[i][1]);
}

void 
print(int N, double *src)
{
    double max = 0.0;
    double min = 0.0;
    for(int i = 0; i < N; i++) {
        max = src[i] > max ? src[i] : max;
        min = src[i] < min ? src[i] : min;
    }

    printf("\n min-max (%f, %f)\n", min, max);
}

void 
clamp(int N, double a, double b, fftw_complex *src, fftw_complex *dst)
{
    for(int i = 0; i < N; i++) 
        dst[i][0] = src[i][0] < a ? a : (src[i][0] > b ? b : src[i][0]);
}

void scale(int N, double c, fftw_complex *src, fftw_complex *dst)
{
    // Scales the real component in place by a factor c.
    for(int i = 0; i < N; i++)
        dst[i][0] = src[i][0] * c;
}

void scale(int N, double c, double *src, double *dst)
{
    for(int i = 0; i < N; i++)
        dst[i] = src[i] * c;
}

void 
abs(int N, double *src, double *dst)
{
    // Takes the absolute value of the real component
    for(int i = 0; i < N; i++)
        dst[i] = abs(src[i]);
}

void 
normalizeIFFT(int N, double *src, double *dst) {
    // Scales the real component by the product of the dimension sizes.
    scale(N, 1.0/N, src, dst);
}

void
normalize(int N, fftw_complex *src, fftw_complex *dst) {
    // Scales the real component to [0, 1]
    double max = 0.0;
    double min = 0.0;
    for(int i = 0; i < N; i++) {
        max = src[i][0] > max ? src[i][0] : max;
        min = src[i][0] < min ? src[i][0] : min;
    }
    for(int i = 0; i < N; i++)
        dst[i][0] = (src[i][0]-min)/(max-min);
}

void
normalize(int N, double *src, double *dst) {
    // Scales to [0, 1]
    double max = 0.0;
    double min = 0.0;
    for(int i = 0; i < N; i++) {
        max = src[i] > max ? src[i] : max;
        min = src[i] < min ? src[i] : min;
    }
    for(int i = 0; i < N; i++)
        dst[i] = (src[i]-min)/(max-min);
}

void 
cartToPolar(int N, fftw_complex *src, fftw_complex *dst) {
    // (x, y) -> (r, φ)
    for(int i = 0; i < N; i++) {
        double x = src[i][0];
        double y = src[i][1];
        dst[i][0] = sqrt(x*x+y*y);
        dst[i][1] = atan2(y, x);
    }
}

void 
polarToCart(int N, fftw_complex *src, fftw_complex *dst) {
    // (r, φ) -> (x, y)
    for(int i = 0; i < N; i++) {
        double r = src[i][0];
        double phi = src[i][1];
        dst[i][0] = r * cos(phi);
        dst[i][1] = r * sin(phi);
    }
}

void 
shiftCoord(int n, int i, int j, int &ii, int &jj)
{
    // Accomplishes the same as numpy.fft.fftshift, converts a coordinate as if zero-freq is at center of spectrum
    // i.e. [0, 0] -> [n/2, n/2]
    assert(n % 2 == 0);
    ii = i >= n/2 ? i - n/2 : i + n/2;
    jj = j >= n/2 ? j - n/2 : j + n/2;
}

double 
signalPower(int N, fftw_complex *src)
{
    // Signal power, sum(m^2), assuming in magnitude-phase form.
    double power = 0;
    for(int i = 0; i < N; i++)
        power += src[i][0]*src[i][0];
    return power;
}

float 
PMSpectrum2D(float f, float theta)
{
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
slow_fft_phase_shift(int n, double *src, double *dst) {
    int N = n*n;
    auto fft = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

    slow_fft_forward(n, src, fft);

    cartToPolar(N, fft, fft);
    for(int i = 0; i < N; i++)
        fft[i][1] += 0.04;
    polarToCart(N, fft, fft);

    slow_fft_backward(n, fft, dst);

    fftw_free(fft);
}

void slow_fft_forward(int n, double *src, fftw_complex *dst)
{
    fftw_plan plan = fftw_plan_dft_r2c_2d(n, n, src, dst, FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);
}

void slow_fft_forward(int n, fftw_complex *src, fftw_complex *dst)
{
    fftw_plan plan = fftw_plan_dft_2d(n, n, src, dst, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);
}

void slow_fft_backward(int n, fftw_complex *src, double *dst)
{
    fftw_plan plan = fftw_plan_dft_c2r_2d(n, n, src, dst, FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);
    normalizeIFFT(n*n, dst, dst);
}

void slow_fft_backward(int n, fftw_complex *src, fftw_complex *dst)
{
    fftw_plan plan = fftw_plan_dft_2d(n, n, src, dst, FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);
    scale(n*n, 1.0/(n*n), dst, dst);
}

