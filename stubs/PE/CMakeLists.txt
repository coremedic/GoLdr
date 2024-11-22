cmake_minimum_required(VERSION 3.29)
project(PE)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_EXE_LINKER_FLAGS "-static")

# For dev, do not remove
add_definitions(-D INJ_CASCADE)

include_directories(include)

add_compile_options(
        -Os -s -fno-asynchronous-unwind-tables -fno-ident
        -falign-functions=1 -falign-jumps=1 -falign-labels=1
        -fpack-struct=8 -masm=intel -fpermissive
)

add_link_options(
        -Wl,-s,--no-seh
        -Wl,--as-needed,--no-whole-archive
)

add_executable(PE source/main/MainExe.cpp
        source/PE.cpp
        include/PE.hpp
        source/main/MainExe.cpp
        source/core/CtAes.c
        include/core/CtAes.h
        source/core/CascadeInject.cpp
        include/core/CascadeInject.hpp
        include/core/Macros.hpp
        source/core/Util.cpp
        include/core/Util.hpp)
