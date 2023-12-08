add_executable(boat main.cpp)
target_compile_features(boat PRIVATE cxx_std_17)

target_link_libraries(boat PUBLIC tinyrender)
target_copy_webgpu_binaries(boat)  # Must include!

if (EMSCRIPTEN)
    # Add Emscripten-specific link options
    string(APPEND CMAKE_C_FLAGS " -pthread")
    string(APPEND CMAKE_CXX_FLAGS " -pthread")

    target_link_options(boat PRIVATE
        #-g
        #-O0
        -O3
        -pthread
        -USE_PTHREADS

        #-sPROXY_TO_PTHREAD -- BAD! we need to be on the browser thread so we can do GLFW/WebGPU stuff

        #-sASSERTIONS=2
        #-fsanitize=address
        #-sSTACK_OVERFLOW_CHECK=2

        -sPTHREAD_POOL_SIZE=64

        -sSTACK_SIZE=100MB
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

    set_target_properties(boat PROPERTIES SUFFIX ".html")
endif()

