# glm
add_subdirectory(glm)

# glfw
add_subdirectory(glfw)

# Dear ImGUI
file(GLOB_RECURSE IMGUI_SRC "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.cpp")
file(GLOB_RECURSE IMGUI_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.h")
add_library(imgui STATIC "${IMGUI_SRC}" "${IMGUI_HEADERS}")
target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/imgui")

# assimp
set(BUILD_SHARED_LIBS, ON)
add_subdirectory(assimp)

# freetype
add_subdirectory(freetype) # 2.13.3

# stb
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/stb")
