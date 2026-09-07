cmake_minimum_required(VERSION 4.2)
project(wbtp LANGUAGES C)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Library
add_library(wbtp SHARED)
target_sources(wbtp PUBLIC src/init.c src/sede.c src/net.c src/util.c src/errors.c)
target_include_directories(wbtp PUBLIC include)
target_compile_definitions(wbtp PRIVATE WBTP_BUILD)