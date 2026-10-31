cmake_minimum_required(VERSION 3.16)
project(CarnivalOfTheCursed C CXX)
set(CMAKE_CXX_STANDARD 17)

# Build GLFW from source, skipping its extras
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)

# Every .cpp in src/ is included automatically
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(Carnival ${SOURCES} external/glad/src/glad.c "src/game/Shots.h" "src/game/Shots.cpp"
    external/stb/stb_image.h)

target_include_directories(Carnival PRIVATE
    external
    external/glad/include
    external/glm
    src/core src/game src/characters src/world
    src/ui src/states src/cutscene src/audio
)

find_package(OpenGL REQUIRED)
target_link_libraries(Carnival PRIVATE glfw OpenGL::GL)

# Lets code find fonts/sounds from anywhere
target_compile_definitions(Carnival PRIVATE ASSET_DIR="${CMAKE_SOURCE_DIR}/assets/")

if (APPLE)
    target_compile_definitions(Carnival PRIVATE GL_SILENCE_DEPRECATION)
endif()