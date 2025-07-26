#!/bin/bash

mkdir -p ./builds/web

em++ \
src/audiomanager.cpp \
src/deviceinfo.cpp \
src/game.cpp \
src/main.cpp \
src/scene_splash.cpp \
src/scene_tama.cpp \
src/scenemanager.cpp \
src/tamapetai.cpp \
src/tamaui.cpp \
-I./src/ \
-I/home/aj/Documents/raylib/web/release/include \
-L/home/aj/Documents/raylib/web/release/lib \
-lraylib.web \
-s USE_GLFW=3 \
-s STACK_SIZE=1048576 \
-s INITIAL_MEMORY=234881024 \
-s ASYNCIFY \
-s SINGLE_FILE \
-s ASSERTIONS \
-Oz \
-D PLATFORM_WEB=1 \
--embed-file resources \
-o ./builds/web/tamamatch3.html \
--shell-file ./builds/shell.html

exit 0
