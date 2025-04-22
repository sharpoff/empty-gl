cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)
project(Quiet LANGUAGES CXX)

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "src/*.h")

add_compile_options("$<$<CONFIG:DEBUG>:-Wall>")

add_subdirectory(external)

add_library(engine
    STATIC
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

target_include_directories(engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

find_package(GLEW REQUIRED)
target_link_libraries(engine
    PUBLIC

    GL
    GLEW::GLEW
    glfw
    glm
    imgui
    assimp
    freetype
    stb
)

add_executable(main
    src/main.cpp
)

target_link_libraries(main
    PUBLIC
    engine
)
