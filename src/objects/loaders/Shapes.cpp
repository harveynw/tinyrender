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
            int n_data = sizeof(UVTriangleVertexAttributes) * v.size();

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
