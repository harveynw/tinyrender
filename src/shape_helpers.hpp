#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <webgpu/webgpu.hpp>

#include "webgpu/pipelines/TexturedTrianglePipeline.hpp"
#include "webgpu/pipelines/TrianglePipeline.hpp"

// For generating random numbers
#define r() ((float) rand() / RAND_MAX)
#define rrr() r(), r(), r()

// For manipulating std::vector
#define vec_splat(w) w.x, w.y, w.z
#define vec_append(x, y) x.insert(x.end(), y.begin(), y.end())

// For getting glm::vec's out of std::vector's
#define take_vec3(iterator) glm::vec3(*(iterator), *(iterator+1), *(iterator+2))
#define take_vec2(iterator) glm::vec2(*(iterator), *(iterator+1))

namespace shape_helpers {
    struct ShapeData {
       std::vector<float> positionData;
       std::vector<float> normalData;
       std::vector<float> colorData;
       std::vector<float> uvData;

       std::vector<uint16_t> indexData;

       ShapeData& operator+=(const ShapeData& rhs) {
           std::vector<uint16_t> adjustedIndex = rhs.indexData;
           for(unsigned short &it : adjustedIndex)
               it += nPoints();

           vec_append(positionData, rhs.positionData);
           vec_append(normalData, rhs.normalData);
           vec_append(colorData, rhs.colorData);
           vec_append(uvData, rhs.uvData);
           vec_append(indexData, adjustedIndex);
           return *this;
       }
       friend ShapeData operator+(ShapeData &lhs, const ShapeData &rhs) {
           ShapeData sd = {{}, {}, {}, {}, {}};
           sd += lhs;
           sd += rhs;
           return sd;
       }

       int nPoints() { return (int) positionData.size()/3; }
       int nColors() { return (int) colorData.size()/3; }
       int nIndices() { return (int) indexData.size(); }
   };

   ShapeData makeTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);
   ShapeData makeQuad(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d);
   ShapeData makePyramid(glm::vec3 centre, float height, float width);

   /*
    * The various pipelines and shaders consume data differently, so we put all this awkward logic down here for now
    */

   [[maybe_unused]] static std::vector<TriangleVertexAttributes>
   asTriangles(ShapeData &shapeData) {
       std::vector<TriangleVertexAttributes> v;

       auto it_p = shapeData.positionData.begin();
       auto it_n = shapeData.normalData.begin();
       auto it_c = shapeData.colorData.begin();

       for(auto idx : shapeData.indexData) {
           int i = (int) idx;
           auto pos = take_vec3(it_p + i * 3);
           auto normal = take_vec3(it_n + i * 3);
           auto color = take_vec3(it_c + i * 3);
           printf("%f, %f, %f\n", pos.x, pos.y, pos.z);
           v.push_back({pos, normal, color});
       }

       return v;
   }

    [[maybe_unused]] static std::vector<UVTriangleVertexAttributes>
   asTexturedTriangles(ShapeData &shapeData) {
       std::vector<UVTriangleVertexAttributes> v;

       auto it_p = shapeData.positionData.begin();
       auto it_n = shapeData.normalData.begin();
       auto it_c = shapeData.colorData.begin();
       auto it_uv = shapeData.uvData.begin();

       for(auto idx : shapeData.indexData) {
           int i = (int) idx;
           auto pos = take_vec3(it_p + 3 * i);
           auto normal = take_vec3(it_n + 3 * i);
           auto color = take_vec3(it_c + 3 * i);
           auto uv = take_vec2(it_uv + 2 * i);
           v.push_back({pos, normal, color, uv});
       }

       return v;
   }
}
