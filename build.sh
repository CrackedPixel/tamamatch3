#!/bin/bash

mkdir -p ./builds/web

em++ \
src/main.cpp \
src/game.cpp \
-I. \
-I/home/aj/Documents/raylib/web/include \
-L/home/aj/Documents/raylib/web/lib \
-lraylib \
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
