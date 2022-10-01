if exist cmake-build-web rd /s /q cmake-build-web
mkdir cmake-build-web
robocopy assets cmake-build-web/assets /E
cmake -S . -B cmake-build-web -G Ninja "-DCMAKE_TOOLCHAIN_FILE=C:/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" -DPLATFORM=Web
cmake --build cmake-build-web