#!/bin/sh

INCLUDES="-I../include"
LINKER_FLAGS="-L../libs -lglfw3 -framework OpenGL -framework Cocoa -framework IOkit -framework CoreVideo"
SOURCES="*.c ../src/*/*.c"

clang $SOURCES $INCLUDES $LINKER_FLAGS -Wall -g -o shaders.out