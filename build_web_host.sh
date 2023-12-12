# Builds all the examples using emscripten and then starts a webserver pointing to the build folder
emcmake cmake -B build-web
cmake --build build-web
python3 web_host.py