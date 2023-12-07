#include "Shapes.hpp"

// For generating random numbers
#define r() ((float) rand() / RAND_MAX)
#define rrr() r(), r(), r()

// For manipulating std::vector
#define vec_splat(w) w.x, w.y, w.z
#define vec_append(x, y) x.insert(x.end(), y.begin(), y.end())

// For getting glm::vec's out of std::vector's
#define take_vec3(iterator) glm::vec3(*(iterator), *(iterator+1), *(iterator+2))
#define take_vec2(iterator) glm::vec2(*(iterator), *(iterator+1))

namespace {
    struct ShapeData {
        std::vector<float> positionData;
        std::vector<float> normalData;
        std::vector<float> colorData;
        std::vector<float> uvData;

        std::vector<uint16_t> indexData;

        ShapeData &operator+=(const ShapeData &rhs) {
            std::vector<uint16_t> adjustedIndex = rhs.indexData;
            for (unsigned short &it: adjustedIndex)
                it += nPoints();

            vec_append(positionData, rhs.positionData);
            vec_append(normalData, rhs.normalData);
            vec_append(colorData, rhs.colorData);
            vec_append(uvData, rhs.uvData);
            vec_append(indexData, adjustedIndex);
            return *this;
        }

        [[maybe_unused]]
        friend ShapeData operator+(ShapeData &lhs, const ShapeData &rhs) {
            ShapeData sd = {{},
                            {},
                            {},
                            {},
                            {}};
            sd += lhs;
            sd += rhs;
            return sd;
        }

        void asPolygons(Polygons &p) {
            std::vector<UVTriangleVertexAttributes> v;

            auto it_p = this->positionData.begin();
            auto it_n = this->normalData.begin();
            auto it_c = this->colorData.begin();
            auto it_uv = this->uvData.begin();

            for(auto idx : this->indexData) {
                int i = (int) idx;
                auto pos = take_vec3(it_p + 3 * i);
                auto normal = take_vec3(it_n + 3 * i);
                auto color = take_vec3(it_c + 3 * i);
                auto uv = take_vec2(it_uv + 2 * i);
                v.push_back({pos, normal, color, uv});
            }

            auto *data = reinterpret_cast<float *>(v.data()); // Flatten
            int n_data = (int) sizeof(UVTriangleVertexAttributes)/sizeof(float) * (int) v.size();
            p.data = std::vector<float>(data, data + n_data);
            p.vertices = v.size();
        }

        int nPoints() { return (int) positionData.size() / 3; }
    };

    ShapeData makeTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
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

    ShapeData makeQuad(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d) {
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

    ShapeData makePyramid(glm::vec3 centre, float height, float width) {
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
}

void
loadTriangle(Polygons &p, glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    auto shape = makeTriangle(a, b, c);
    shape.asPolygons(p);
}

void
loadQuad(Polygons &p, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d) {
    auto shape = makeQuad(a, b, c, d);
    shape.asPolygons(p);
}

void
loadPyramid(Polygons &p, glm::vec3 centre, float height, float width) {
    auto shape = makePyramid(centre, height, width);
    shape.asPolygons(p);
}

void
loadPlaneMesh(Polygons &p, float width, float length, int nWidth, int nLength) {
    float dx = width/(float) nWidth;
    float dy = length/(float) nLength;

    // Grid coordinate [0, nWidth] x [0, nLength] to world coordinate R x R x {0}
    auto coordToPosition = [width, length, dx, dy](int x, int y) {
        return glm::vec3((x * dx) - width/2.0, (y * dy) - length/2.0, 0.0);
    };

    auto v = std::vector<UVTriangleVertexAttributes>(6 * nWidth * nLength);
    for(int y = 0; y < nLength; y++) {
        for(int x = 0; x < nWidth; x++) {
            auto it = v.begin() + 6 * (y * nWidth + x);

            // Grid square in world positions
            auto a = coordToPosition(x, y);
            auto b = coordToPosition(x+1, y);
            auto c = coordToPosition(x+1, y+1);
            auto d = coordToPosition(x, y+1);

            (*it).position = a;
            (*(it+1)).position = b;
            (*(it+2)).position = d;

            (*(it+3)).position = d;
            (*(it+4)).position = b;
            (*(it+5)).position = c;

            (*it).uv = glm::vec2(x/(float) nWidth, y/ (float) nLength);
            (*(it+1)).uv = glm::vec2((x+1)/(float) nWidth, y/ (float) nLength);
            (*(it+2)).uv = glm::vec2(x/(float) nWidth, (y+1)/ (float) nLength);

            (*(it+3)).uv = glm::vec2(x/(float) nWidth, (y+1)/ (float) nLength);
            (*(it+4)).uv = glm::vec2((x+1)/(float) nWidth, y/ (float) nLength);
            (*(it+5)).uv = glm::vec2((x+1)/(float) nWidth, (y+1)/ (float) nLength);

            (*it).normal = glm::vec3(0, 0, 1);
            (*(it+1)).normal = glm::vec3(0, 0, 1);
            (*(it+2)).normal = glm::vec3(0, 0, 1);

            (*(it+3)).normal = glm::vec3(0, 0, 1);
            (*(it+4)).normal = glm::vec3(0, 0, 1);
            (*(it+5)).normal = glm::vec3(0, 0, 1);
        };
    }

    auto *data = reinterpret_cast<float *>(v.data()); // Flatten
    int n_data = sizeof(UVTriangleVertexAttributes)/sizeof(float) * v.size();

    p.data = std::vector<float>(data, data + n_data);
    p.vertices = v.size();
}
