//
// Created by Harvey Williams on 05/08/2023.
//

#include "shape_helpers.hpp"
#include <gtest/gtest.h>

using namespace shape_helpers;
using namespace glm;

// TODO Split these up to one assert per test

TEST(ShapeHelpersTests, ShapeDataConcatenation){
    ShapeData tri_1 = makeTriangle(vec3(0, 0, 0), vec3(1, 1, 0), vec3(1, 1, 1));
    ShapeData tri_2 = makeTriangle(vec3(2, 0, 0), vec3(2, 2, 0), vec3(2, 2, 2));

    ShapeData tri_3 = tri_1 + tri_2;

    EXPECT_EQ(tri_3.nColors(), tri_3.nPoints()) << "Expected as many colors specified as vertices";

    size_t expSize = tri_1.pointData.size() + tri_2.pointData.size();
    EXPECT_EQ(tri_3.pointData.size(), expSize) << "Vertex point data is not expected size";

    std::vector<uint16_t> adjIndexData(tri_3.indexData.begin() + 3, tri_3.indexData.end());
    EXPECT_NE(adjIndexData, tri_2.indexData) << "Index data has not been adjusted post-compound";

    ShapeData compAssigned = {{}, {}};
    compAssigned += tri_1;
    compAssigned += tri_2;

    auto msg = "Compound assign behaves differently to compound\n";
    EXPECT_EQ(compAssigned.indexData, tri_3.indexData) << msg;
    EXPECT_EQ(compAssigned.pointData, tri_3.pointData) << msg;
}
