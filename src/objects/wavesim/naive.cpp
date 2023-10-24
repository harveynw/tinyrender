#include "naive.hpp"

void
NaiveWaveSim::update(float dt, std::vector<uint8_t> &displacementData) {
    //this->time += 0.05f * dt;
    (void) dt;

    float waves_s = 1.0;
    float waves_c = 1.0;
    float time_speed = waves_s * 100;

    for (int i = 0; i < DISPLACE_MAP_RES; ++i) {
        for (int j = 0; j < DISPLACE_MAP_RES; ++j) {
            float x = i/5.0;
            float y = j/5.0;
            uint8_t *p = &displacementData[4 * (j * DISPLACE_MAP_RES + i)];
            // Only store displacement in the first channel
            float var = (glm::sin(waves_s * (x + time_speed * this->time)) + glm::cos(waves_c * (y + time_speed * this->time)));
            //var = (float)i/(float)DISPLACE_MAP_RES;
            p[0] = (uint8_t) (255 * (var + 2.0)/4.0);
            //p[0] = (uint8_t) (255 * var);
        }
    }
}
