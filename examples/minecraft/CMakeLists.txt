# Define target
add_executable(minecraft main.cpp)
target_compile_features(minecraft PRIVATE cxx_std_17)

# Link tinyrender
target_link_libraries(minecraft PUBLIC tinyrender)
target_copy_webgpu_binaries(minecraft)  # Must include!

# Link with FastNoiseLite for terrain generation
FetchContent_Declare(
        FastNoiseLite
        URL https://github.com/Auburn/FastNoiseLite/archive/refs/tags/v1.1.0.zip
)
FetchContent_MakeAvailable(FastNoiseLite)
target_include_directories(minecraft PRIVATE ${fastnoiselite_SOURCE_DIR}/Cpp)

# Support emscripten
if (EMSCRIPTEN)
    # Add Emscripten-specific link options
    string(APPEND CMAKE_C_FLAGS " -pthread")
    string(APPEND CMAKE_CXX_FLAGS " -pthread")

    target_link_options(minecraft PRIVATE
        -O3
        -pthread
        -USE_PTHREADS
        -sPTHREAD_POOL_SIZE=64
        -sSTACK_SIZE=100MB
        -sUSE_GLFW=3 # Use Emscripten-provided GLFW
        -sUSE_WEBGPU # Handle WebGPU symbols
        -sASYNCIFY # Required by WebGPU-C++
        -sALLOW_MEMORY_GROWTH
        -sINITIAL_MEMORY=3GB
        -sMAXIMUM_MEMORY=4GB
    )

    set_target_properties(minecraft PROPERTIES SUFFIX ".html")
endif()

