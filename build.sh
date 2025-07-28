#!/bin/bash

mkdir -p ./builds/web

em++ \
src/audiomanager.cpp \
src/deviceinfo.cpp \
src/game.cpp \
src/main.cpp \
src/scene_credits.cpp \
src/scene_menu.cpp \
src/scene_splash.cpp \
src/scene_tama.cpp \
src/scenemanager.cpp \
src/tamapetai.cpp \
src/tamaui.cpp \
-I./src/ \
-I./external/ \
-I/home/aj/Documents/raylib/web/release/include \
-L/home/aj/Documents/raylib/web/release/lib \
-lraylib.web \
-s USE_GLFW=3 \
-s SINGLE_FILE \
-s INITIAL_MEMORY=65536000 \
-s EXPORTED_RUNTIME_METHODS=['HEAPF32'] \
-s ASSERTIONS \
-s ASYNCIFY \
-Oz \
-D PLATFORM_WEB=1 \
-D USE_EXTERNAL_GLFW=ON \
--embed-file resources \
-o ./builds/web/tamagacha.html \
--shell-file ./builds/shell.html

exit 0

# -s STACK_SIZE=1048576 \
# -s INITIAL_MEMORY=234881024 \
# -s ASYNCIFY \
