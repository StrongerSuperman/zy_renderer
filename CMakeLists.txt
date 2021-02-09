cmake_minimum_required(VERSION 3.10)
project(zy_renderer)


set(HEADERS
    src/platform.hpp
    src/win32.hpp
    src/camera.hpp
    src/graphics.hpp
    src/image.hpp
    src/shader.hpp
    src/texture.hpp
    src/mathtool.hpp
)
set(SOURCES
    src/win32.cpp
    src/camera.cpp
    src/graphics.cpp
    src/image.cpp
    src/shader.cpp
    src/texture.cpp
)

set(SOURCES ${SOURCES} src/main.cpp)

set(TARGET zy_renderer)

add_executable(${TARGET} ${HEADERS} ${SOURCES})

set_target_properties(zy_renderer PROPERTIES 
    CMAKE_CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

include_directories(${CMAKE_SOURCE_DIR}/3rd)

target_compile_options(${TARGET} PRIVATE /W4 /D_CRT_SECURE_NO_WARNINGS)
target_compile_options(${TARGET} PRIVATE /fp:fast)

set_directory_properties(PROPERTIES VS_STARTUP_PROJECT ${TARGET})
source_group(TREE "${CMAKE_SOURCE_DIR}/src" FILES ${HEADERS} ${SOURCES})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
