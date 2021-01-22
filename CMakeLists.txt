cmake_minimum_required(VERSION 3.10)
project(zy_renderer)


set(HEADERS
    src/graphics.hpp

    src/platform.hpp
    src/win32.hpp
)
set(SOURCES
    src/graphics.cpp
    src/win32.cpp
    src/win32_func.cpp
)

set(SOURCES ${SOURCES} src/main.cpp)

set(TARGET zy_renderer)

add_executable(${TARGET} ${HEADERS} ${SOURCES})

set_target_properties(zy_renderer PROPERTIES 
    CMAKE_CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_compile_options(${TARGET} PRIVATE /W4 /D_CRT_SECURE_NO_WARNINGS)
target_compile_options(${TARGET} PRIVATE /fp:fast)

set_directory_properties(PROPERTIES VS_STARTUP_PROJECT ${TARGET})
source_group(TREE "${CMAKE_SOURCE_DIR}/src" FILES ${HEADERS} ${SOURCES})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
