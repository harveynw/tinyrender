#pragma once

#include <vector>
#include <glm/glm.hpp>

/*
* Naive wave sim
*/

class NaiveWaveSim {
protected:
    int DISPLACE_MAP_RES;
    float time = 0.0;
public:
    NaiveWaveSim(int res): DISPLACE_MAP_RES(res){};
    void update(float dt, std::vector<uint8_t> &displacementData);
};