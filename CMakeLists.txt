cmake_minimum_required(VERSION 3.10)
project(zy_renderer)


set(HEADERS
    src/platform/win32.hpp

    src/core/platform.hpp
    src/core/camera.hpp
    src/core/graphics.hpp
    src/core/perframe.hpp
    src/core/program.hpp
    src/core/framebuffer.hpp
    src/core/image.hpp
    src/core/shader.hpp
    src/core/texture.hpp
    src/core/math_tool.hpp
    src/core/mesh.hpp
    src/core/scene.hpp

    src/blinn/blinn_material.hpp
    src/blinn/blinn_model.hpp
    src/blinn/blinn_scene.hpp
    src/blinn/blinn_shader.hpp
)
set(SOURCES
    src/platform/win32.cpp

    src/core/camera.cpp
    src/core/graphics.cpp
    src/core/framebuffer.cpp
    src/core/image.cpp
    src/core/texture.cpp
    src/core/mesh.cpp

    src/blinn/blinn_material.cpp
    src/blinn/blinn_model.cpp
    src/blinn/blinn_scene.cpp
    src/blinn/blinn_shader.cpp
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
