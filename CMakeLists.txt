cmake_minimum_required(VERSION 3.0...3.25)
include(FetchContent)

project(
        tinyrender
        VERSION 0.1.0
        LANGUAGES CXX C
)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
        # TODO test here

        # TODO Doxygen
        # find_package(Doxygen)
        # if(Doxygen_FOUND)
        # add_subdirectory(docs)
        # else()
        # message(STATUS "Doxygen not found, not building docs")
        # endif()
endif()

# Includes tinyrender as a target
add_subdirectory(src)

# Test scenes
add_subdirectory(examples/TestScene)

#
# TESTING
#
# Get Google Test
#FetchContent_Declare(
#        googletest
#        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
#)
#FetchContent_MakeAvailable(googletest)
#enable_testing()
#include(GoogleTest)
#
#function(add_test name sources)
#    add_executable(${name} ${sources})
#    message(STATUS "adding test" ${sources})
#    target_link_libraries(${name} GTest::gtest_main glm::glm webgpu glfw glfw3webgpu stb_image_lib)
#    target_copy_webgpu_binaries(${name})
#    gtest_discover_tests(${name})
#endfunction()

#add_test(shape_helpers_test "src/shape_helpers_test.cpp;src/shape_helpers.hpp;src/shape_helpers.cpp")
