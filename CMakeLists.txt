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

# if (EMSCRIPTEN)
#     # Add Emscripten-specific link options
#     target_link_options(App PRIVATE
#         -sUSE_GLFW=3 # Use Emscripten-provided GLFW
#         -sUSE_WEBGPU # Handle WebGPU symbols
#         -sASYNCIFY # Required by WebGPU-C++
#         -sALLOW_MEMORY_GROWTH
#         -sINITIAL_MEMORY=3GB
#         -sMAXIMUM_MEMORY=4GB
#         --preload-file "../resources"
#         )
#     # For debugging:
#     # -O0
#     # -g
#     #-sASSERTIONS=2
#         #-fsanitize=address
#     #-sSAFE_HEAP=1
#     #-sSTACK_OVERFLOW_CHECK=1
# 
#     set_target_properties(App PROPERTIES SUFFIX ".html")
# endif()

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
