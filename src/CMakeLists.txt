file(GLOB SOURCE_LIST
    *.cpp
    objects/*.cpp
    objects/loaders/*.cpp
    objects/wavesim/*.cpp
    objects/voxel/*.cpp
    webgpu/*.cpp
    webgpu/pipelines/*.cpp
    webgpu/primitives/shaders/*.cpp
    webgpu/primitives/uniforms/*.cpp
    webgpu/primitives/buffers/*.cpp
    webgpu/primitives/buffers/attributes/*.cpp
    webgpu/primitives/buffers/uniforms/*.cpp
    webgpu/primitives/textures/*.cpp
    webgpu/primitives/textures/utilities/*.cpp
    camera/*.cpp
)
list(FILTER SOURCE_LIST EXCLUDE REGEX "_test.cpp$")

# if(EMSCRIPTEN)
#         string(APPEND CMAKE_C_FLAGS " -pthread")
#         string(APPEND CMAKE_CXX_FLAGS " -pthread")
# endif()

# Declare tinyrender library to link to
add_library(tinyrender STATIC ${SOURCE_LIST})
target_include_directories(tinyrender PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/camera
        ${CMAKE_CURRENT_SOURCE_DIR}/objects
        ${CMAKE_CURRENT_SOURCE_DIR}/webgpu
)

get_filename_component(TINY_ROOT ../ ABSOLUTE)
target_include_directories(tinyrender PRIVATE ${TINY_ROOT}/include/tinyrender)
target_include_directories(tinyrender PUBLIC ${TINY_ROOT}/include)

# Make sure to show as many warnings as possible
set_target_properties(tinyrender PROPERTIES
        CXX_STANDARD 17
        COMPILE_WARNING_AS_ERROR ON
        )
if (MSVC)
    target_compile_options(tinyrender PRIVATE /W4)
else()
    target_compile_options(tinyrender PRIVATE -Wall -Wextra -pedantic)
endif()

# GLFW for opening a window
if (NOT EMSCRIPTEN)
FetchContent_Declare(
        glfw3
        URL https://github.com/glfw/glfw/releases/download/3.3.8/glfw-3.3.8.zip
)
FetchContent_MakeAvailable(glfw3)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
target_link_libraries(tinyrender PUBLIC glfw)
endif()

# WebGPU implementation and headers
set(WEBGPU_BACKEND "WGPU" CACHE STRING "" FORCE)
FetchContent_Declare(
        webgpu
        GIT_REPOSITORY https://github.com/eliemichel/WebGPU-distribution
        GIT_TAG        main
)
FetchContent_MakeAvailable(webgpu)
target_link_libraries(tinyrender PUBLIC webgpu)

# GLFW extension for getting a webgpu surface
FetchContent_Declare(
        glfw3webgpu
        GIT_REPOSITORY https://github.com/eliemichel/glfw3webgpu
        GIT_TAG        main
)
FetchContent_MakeAvailable(glfw3webgpu)
target_link_libraries(tinyrender PRIVATE glfw3webgpu)

# GLM for matrix math
FetchContent_Declare(
        glm
        URL https://github.com/g-truc/glm/archive/refs/tags/0.9.9.8.zip
)
FetchContent_MakeAvailable(glm)
target_link_libraries(tinyrender PUBLIC glm::glm)

# STB_Image
FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb.git
        GIT_TAG master
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
)
FetchContent_MakeAvailable(stb)
target_include_directories(tinyrender PRIVATE ${stb_SOURCE_DIR})

# TinyOBJLoader (a single header file)
FetchContent_Declare(
        tinyobjloader
        GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
        GIT_TAG release
)
FetchContent_MakeAvailable(tinyobjloader)
target_link_libraries(tinyrender PRIVATE tinyobjloader)

# FFTW 
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
        fftw3
        URL https://www.fftw.org/fftw-3.3.10.tar.gz
        URL_HASH MD5=8ccbf6a5ea78a16dbc3e1306e234cc5c
)
FetchContent_MakeAvailable(fftw3)
target_include_directories(tinyrender PRIVATE ${fftw3_SOURCE_DIR}/api)
set_property(TARGET fftw3 PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(tinyrender PRIVATE fftw3)

# FastNoiseLite for voxel terrain generation
FetchContent_Declare(
        FastNoiseLite
        URL https://github.com/Auburn/FastNoiseLite/archive/refs/tags/v1.1.0.zip
)
FetchContent_MakeAvailable(FastNoiseLite)
target_include_directories(tinyrender PRIVATE ${fastnoiselite_SOURCE_DIR}/Cpp)
