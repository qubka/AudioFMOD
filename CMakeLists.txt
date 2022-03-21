cmake_minimum_required(VERSION 3.20)
project(3dAudio)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SRC_SOURCES src/*.cpp)
file(GLOB_RECURSE SRC_HEADERS src/*.hpp)
set(HEADER_FILES pch.hpp)

add_executable(${PROJECT_NAME} ${SRC_SOURCES} ${SRC_HEADERS} ${HEADER_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
        external
        ${OPENGL_INCLUDE_DIR}
        )

target_link_libraries(${PROJECT_NAME} PUBLIC
        # static link
        glfw
        glm
        glad
        stb
        entt
        magic_enum
        assimp
        freetype
        openal

        # dynamic link
        OpenGL::GL
        )

target_precompile_headers(${PROJECT_NAME} PUBLIC ${HEADER_FILES})