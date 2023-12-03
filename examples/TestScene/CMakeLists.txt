
add_executable(TestScene main.cpp)
target_compile_features(TestScene PRIVATE cxx_std_17)

target_link_libraries(TestScene PUBLIC tinyrender)
target_copy_webgpu_binaries(TestScene)  # Must include!

if (EMSCRIPTEN)
    # Add Emscripten-specific link options
    # string(APPEND CMAKE_C_FLAGS " -pthread")
    # string(APPEND CMAKE_CXX_FLAGS " -pthread")

    target_link_options(TestScene PRIVATE
        -g
        -O0
        #-pthread
        #-USE_PTHREADS
        #-sPROXY_TO_PTHREAD
        -fsanitize=address
        -sUSE_GLFW=3 # Use Emscripten-provided GLFW
        -sUSE_WEBGPU # Handle WebGPU symbols
        #-sWASM_WORKERS
        -sASYNCIFY # Required by WebGPU-C++
        -sALLOW_MEMORY_GROWTH
        -sINITIAL_MEMORY=3GB
        -sMAXIMUM_MEMORY=4GB
        --preload-file "${CMAKE_CURRENT_SOURCE_DIR}/resources@/resources"
    )
    # For debugging:
    # -O0
    # -g
    #-sASSERTIONS=2
        #-fsanitize=address
    #-sSAFE_HEAP=1
    #-sSTACK_OVERFLOW_CHECK=1

    set_target_properties(TestScene PROPERTIES SUFFIX ".html")
endif()

