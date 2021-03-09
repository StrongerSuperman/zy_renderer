cmake_minimum_required(VERSION 3.10)
project(zy_renderer)
set(DEBUG_MODE 1)

set(TARGET zy_renderer)

set(HEADERS)
set(SOURCES)
file(GLOB_RECURSE HEADERS ${CMAKE_SOURCE_DIR}/engine/include/*.hpp)
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/engine/src/*.cpp)

include_directories(${CMAKE_SOURCE_DIR}/engine/3rd)
include_directories(${CMAKE_SOURCE_DIR}/engine/include)

#source_group(TREE "${CMAKE_SOURCE_DIR}/engine/src" FILES ${HEADERS} ${SOURCES})
source_group(TREE "${CMAKE_SOURCE_DIR}/engine" FILES ${HEADERS} ${SOURCES})

add_executable(${TARGET} ${HEADERS} ${SOURCES})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

set_target_properties(zy_renderer PROPERTIES 
    CMAKE_CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

if(DEBUG_MODE)
    set(CMAKE_BUILD_TYPE "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    target_link_libraries(${TARGET}
        "${CMAKE_SOURCE_DIR}/Engine/3rd/assimp/lib/assimp-vc142-mtd.lib"
    )
    add_custom_command(TARGET zy_renderer POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Engine/3rd/assimp/dll/assimp-vc142-mtd.dll"
        ${CMAKE_SOURCE_DIR}/bin/$<CONFIGURATION>
    )
else()
    set(CMAKE_BUILD_TYPE "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
    target_link_libraries(${TARGET}
        "${CMAKE_SOURCE_DIR}/Engine/3rd/assimp/lib/assimp-vc142-mt.lib"
    )
    add_custom_command(TARGET zy_renderer POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Engine/3rd/assimp/dll/assimp-vc142-mt.dll"
        ${CMAKE_SOURCE_DIR}/bin/$<CONFIGURATION>
    )
endif()

target_compile_options(${TARGET} PRIVATE /W4 /D_CRT_SECURE_NO_WARNINGS)
target_compile_options(${TARGET} PRIVATE /fp:fast)

set_directory_properties(PROPERTIES VS_STARTUP_PROJECT ${TARGET})
