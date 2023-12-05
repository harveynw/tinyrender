emcmake cmake -B build-web
cmake --build build-web
# python3 -m http.server -d build-web/examples/TestScene
# http-server build-web/examples/TestScene -p 8000 --cors 
python3 web_host.py