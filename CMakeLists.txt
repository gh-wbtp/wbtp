cmake_minimum_required(VERSION 4.2)
project(wbtp LANGUAGES C)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Library
set(WBTP_SOURCES src/init.c src/sede.c src/net.c src/util.c src/errors.c)

add_library(wbtp STATIC ${WBTP_SOURCES})
target_include_directories(wbtp PUBLIC include)
target_compile_definitions(wbtp PRIVATE WBTP_STATIC)

add_library(wbtp_shared SHARED ${WBTP_SOURCES})
target_include_directories(wbtp_shared PUBLIC include)
target_compile_definitions(wbtp_shared PRIVATE WBTP_BUILD)
set_target_properties(wbtp_shared PROPERTIES OUTPUT_NAME "wbtp")

if(WIN32)
    target_link_libraries(wbtp PRIVATE ws2_32)
    target_link_libraries(wbtp_shared PRIVATE ws2_32)
endif()