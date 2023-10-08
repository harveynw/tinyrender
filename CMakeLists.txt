cmake_minimum_required(VERSION 3.0...3.25)
include(FetchContent)

project(
        tinyrender
        VERSION 0.1.0
        LANGUAGES CXX C
)

# Everything but tests
file(GLOB SOURCES
        src/*.hpp
        src/*.cpp
        src/objects/*.hpp
        src/objects/*.cpp
        src/objects/loaders/*.hpp
        src/objects/loaders/*.cpp
        src/webgpu/*.hpp
        src/webgpu/*.cpp
        src/webgpu/pipelines/*.hpp
        src/webgpu/pipelines/*.cpp
        src/webgpu/primitives/shaders/*.hpp
        src/webgpu/primitives/shaders/*.cpp
        src/webgpu/primitives/buffers/*.hpp
        src/webgpu/primitives/buffers/*.cpp
        src/webgpu/primitives/buffers/attributes/*.hpp
        src/webgpu/primitives/buffers/attributes/*.cpp
        src/webgpu/primitives/buffers/uniforms/*.hpp
        src/webgpu/primitives/buffers/uniforms/*.cpp
        src/webgpu/primitives/textures/*.hpp
        src/webgpu/primitives/textures/*.cpp
        src/webgpu/primitives/textures/utilities/*.hpp
        src/webgpu/primitives/textures/utilities/*.cpp
        src/controllers/*.hpp
        src/controllers/*.cpp
        )
list(FILTER SOURCES EXCLUDE REGEX "_test.cpp$")
list(FILTER SOURCES EXCLUDE REGEX "src/main_old.cpp$")

add_executable(App src/main.cpp ${SOURCES})
set_property(TARGET App PROPERTY CXX_STANDARD 17)

# Make sure to show as many warnings as possible
set_target_properties(App PROPERTIES
        CXX_STANDARD 17
        COMPILE_WARNING_AS_ERROR ON
        )
if (MSVC)
    target_compile_options(App PRIVATE /W4)
else()
    target_compile_options(App PRIVATE -Wall -Wextra -pedantic)
endif()

# GLFW for opening a window
if (NOT EMSCRIPTEN)
FetchContent_Declare(
        glfw3
        URL https://github.com/glfw/glfw/releases/download/3.3.8/glfw-3.3.8.zip
)
FetchContent_MakeAvailable(glfw3)
target_link_libraries(App PRIVATE glfw)
endif()

# WebGPU implementation and headers
FetchContent_Declare(
        webgpu
        GIT_REPOSITORY https://github.com/eliemichel/WebGPU-distribution
        GIT_TAG        main
)
FetchContent_MakeAvailable(webgpu)
target_link_libraries(App PRIVATE webgpu)
target_copy_webgpu_binaries(App) # Dynamic library file included with build for distribution

# GLFW extension for getting a webgpu surface
FetchContent_Declare(
        glfw3webgpu
        GIT_REPOSITORY https://github.com/eliemichel/glfw3webgpu
        GIT_TAG        main
)
FetchContent_MakeAvailable(glfw3webgpu)
target_link_libraries(App PRIVATE glfw3webgpu)

# GLM for matrix math
FetchContent_Declare(
        glm
        URL https://github.com/g-truc/glm/archive/refs/tags/0.9.9.8.zip
)
FetchContent_MakeAvailable(glm)
target_link_libraries(App PRIVATE glm::glm)

# STB_Image
FetchContent_Declare(
        stb_image
        URL https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
        DOWNLOAD_NO_EXTRACT TRUE
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
)
FetchContent_MakeAvailable(stb_image)
add_library(stb_image_lib INTERFACE)
target_include_directories(stb_image_lib INTERFACE ${stb_image_SOURCE_DIR})
target_link_libraries(App PRIVATE stb_image_lib)

# TinyOBJLoader (a single header file)
FetchContent_Declare(
        tinyobjloader
        GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
        GIT_TAG release
)
FetchContent_MakeAvailable(tinyobjloader)
target_link_libraries(App PRIVATE tinyobjloader)

if(NOT EMSCRIPTEN)
# OpenCV for fft
find_package(OpenCV)
target_link_libraries(App PRIVATE ${OpenCV_LIBS})
endif()

# At the end of the CMakeLists.txt
if (EMSCRIPTEN)
    # Add Emscripten-specific link options
    target_link_options(App PRIVATE
            -g
            -sUSE_GLFW=3 # Use Emscripten-provided GLFW
            -sUSE_WEBGPU # Handle WebGPU symbols
            -sASYNCIFY # Required by WebGPU-C++
            -sALLOW_MEMORY_GROWTH
            -sINITIAL_MEMORY=3GB
            -sMAXIMUM_MEMORY=4GB
            --preload-file "../resources"
            )
    # For debugging:
    #-sASSERTIONS=2
    #-sSAFE_HEAP=1
    #-sSTACK_OVERFLOW_CHECK=1

    set_target_properties(App PROPERTIES SUFFIX ".html")
endif()

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
