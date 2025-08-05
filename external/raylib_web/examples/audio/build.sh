#!/bin/bash

CC="emcc"

INCLUDES=(
    -I/home/aj/Documents/raylib/web/include
)

LIBRARIES=(
    -L/home/aj/Documents/raylib/web/lib
)

LIBS=(
    -lraylib
    -lm
)

EMFLAGS=(
    -s USE_GLFW=3
    -s ASYNCIFY
    -s STACK_SIZE=98304
    -O2
    -D PLATFORM_WEB=1
    --preload-file resources
)

mkdir -p out

for file in *.c; do
    filename=$(basename "$file" .c)
    
    # Compile the file, redirecting output to /dev/null
    ${CC} "${INCLUDES[@]}" -o "./build/$filename.html" "$file" "${LIBRARIES[@]}" "${LIBS[@]}" "${EMFLAGS[@]}" # > /dev/null 2>&1

    if [ $? -eq 0 ]; then
        echo "Successfully compiled $file to ./build/$filename"
    else
        echo "Failed to compile $file"
    fi
done
