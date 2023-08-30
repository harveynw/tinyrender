//
// Created by Harvey Williams on 05/08/2023.
//

#include "shape_helpers.hpp"
#include <map>

using namespace shape_helpers;


ShapeData
shape_helpers::makeTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    // https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal
    glm::vec3 n = glm::cross(b - a, c - a);
    return {
            {
                vec_splat(a),
                vec_splat(b),
                vec_splat(c)
            },
            {
                vec_splat(n),
                vec_splat(n),
                vec_splat(n)
            },
            {
                rrr(), rrr(), rrr()
            },
            {
                0.0, 0.0,
                1.0, 0.0,
                0.0, 1.0
            },
            {
                0, 1, 2
            },
    };
}

ShapeData
shape_helpers::makeQuad(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d) {
    // https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal
    glm::vec3 n = glm::cross(b - a, c - a);
    return {
            {
                vec_splat(a),
                vec_splat(b),
                vec_splat(c),
                vec_splat(d)
            },
            {
                vec_splat(n),
                vec_splat(n),
                vec_splat(n),
                vec_splat(n)
            },
            {
                rrr(), rrr(), rrr(), rrr()
            },
            {
                0.0, 0.0,
                1.0, 0.0,
                1.0, 1.0,
                0.0, 1.0
            },
            {
                0, 1, 2,
                0, 2, 3
            }
    };
}

ShapeData
shape_helpers::makePyramid(glm::vec3 centre, float height, float width) {
    float w2 = width/2.0f;

    ShapeData shape = {{}, {}, {}, {}, {}};

    glm::vec3 a = centre+glm::vec3(-w2, -w2, 0);
    glm::vec3 b = centre+glm::vec3(w2, -w2, 0);
    glm::vec3 c = centre+glm::vec3(w2, w2, 0);
    glm::vec3 d = centre+glm::vec3(-w2, w2, 0);
    glm::vec3 tip = centre+glm::vec3(0, 0, height);

    // Base
    shape += makeQuad(a, b, c, d);

    // Edges (cannot be combined as they need their own normals)
    shape += makeTriangle(a, b, tip);
    shape += makeTriangle(b, c, tip);
    shape += makeTriangle(c, d, tip);
    shape += makeTriangle(d, a, tip);

    return shape;
}
