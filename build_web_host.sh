emcmake cmake -B build-web
cmake --build build-web
python3 -m http.server -d build-web