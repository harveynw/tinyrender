add_executable(TestScene main.cpp)
target_compile_features(TestScene PRIVATE cxx_std_17)

target_link_libraries(TestScene PUBLIC tinyrender)
target_copy_webgpu_binaries(TestScene)  # Must include!