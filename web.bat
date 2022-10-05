if exist cmake-build-web rd /s /q cmake-build-web
mkdir cmake-build-web
robocopy assets cmake-build-web/assets /E
cmake -S . -B cmake-build-web -G Ninja "-DCMAKE_TOOLCHAIN_FILE=C:/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" -DPLATFORM=Web
cmake --build cmake-build-web
@echo off
echo ^<!DOCTYPE html^> > cmake-build-web/index.html
echo ^<html^> >> cmake-build-web/index.html
echo ^<head^> >> cmake-build-web/index.html
echo     ^<meta charset="utf-8"^> >> cmake-build-web/index.html
echo     ^<meta http-equiv="Content-Type" content="text/html; charset=utf-8"^> >> cmake-build-web/index.html
echo ^</head^> >> cmake-build-web/index.html
echo ^<body^> >> cmake-build-web/index.html
echo     ^<canvas id="canvas" oncontextmenu="event.preventDefault()"^>^</canvas^> >> cmake-build-web/index.html
echo     ^<script type='text/javascript'^> >> cmake-build-web/index.html
echo         var Module = { >> cmake-build-web/index.html
echo             canvas: (function() { return document.getElementById('canvas'); })() >> cmake-build-web/index.html
echo         }; >> cmake-build-web/index.html
echo     ^</script^> >> cmake-build-web/index.html
echo     ^<script src="LudumDare51.js"^>^</script^> >> cmake-build-web/index.html
echo ^</body^> >> cmake-build-web/index.html
echo ^</html^> >> cmake-build-web/index.html
