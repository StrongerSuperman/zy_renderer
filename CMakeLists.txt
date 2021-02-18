cmake_minimum_required(VERSION 3.10)
project(zy_renderer)


set(HEADERS
    engine/src/platform/win32.hpp

    engine/src/core/platform.hpp
    engine/src/core/camera.hpp
    engine/src/core/graphics.hpp
    engine/src/core/perframe.hpp
    engine/src/core/program.hpp
    engine/src/core/framebuffer.hpp
    engine/src/core/image.hpp
    engine/src/core/shader.hpp
    engine/src/core/texture.hpp
    engine/src/core/math_tool.hpp
    engine/src/core/mesh.hpp
    engine/src/core/scene.hpp

    engine/src/blinn/blinn_material.hpp
    engine/src/blinn/blinn_model.hpp
    engine/src/blinn/blinn_scene.hpp
    engine/src/blinn/blinn_shader.hpp
)
set(SOURCES
    engine/src/platform/win32.cpp

    engine/src/core/camera.cpp
    engine/src/core/graphics.cpp
    engine/src/core/framebuffer.cpp
    engine/src/core/image.cpp
    engine/src/core/texture.cpp
    engine/src/core/mesh.cpp

    engine/src/blinn/blinn_material.cpp
    engine/src/blinn/blinn_model.cpp
    engine/src/blinn/blinn_scene.cpp
    engine/src/blinn/blinn_shader.cpp
)

set(SOURCES ${SOURCES} engine/src/main.cpp)

set(TARGET zy_renderer)

include_directories(${CMAKE_SOURCE_DIR}/engine/3rd)
add_executable(${TARGET} ${HEADERS} ${SOURCES})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
source_group(TREE "${CMAKE_SOURCE_DIR}/engine/src" FILES ${HEADERS} ${SOURCES})

set_target_properties(zy_renderer PROPERTIES 
    CMAKE_CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_compile_options(${TARGET} PRIVATE /W4 /D_CRT_SECURE_NO_WARNINGS)
target_compile_options(${TARGET} PRIVATE /fp:fast)

set_directory_properties(PROPERTIES VS_STARTUP_PROJECT ${TARGET})
