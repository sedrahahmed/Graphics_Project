cmake_minimum_required(VERSION 3.16)
project(CarnivalOfTheCursed C CXX)
set(CMAKE_CXX_STANDARD 17)

# Build GLFW from source, skipping its extras
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)
# FreeType (text rendering): build from source, without optional extras
set(FT_DISABLE_ZLIB     ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BZIP2    ON CACHE BOOL "" FORCE)
set(FT_DISABLE_PNG      ON CACHE BOOL "" FORCE)
set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BROTLI   ON CACHE BOOL "" FORCE) #The FT_DISABLE_... lines turn off optional features (compressed fonts, colour emoji and so on). Otherwise FreeType would look for other libraries you don’t have, and the build would fail.
add_subdirectory(external/freetype) #add_subdirectory builds FreeType from source, the same way your project already builds GLFW. That works in both VS Code and Visual Studio.

# Every .cpp in src/ is included automatically
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(Carnival ${SOURCES} external/glad/src/glad.c "src/game/Shots.h" "src/game/Shots.cpp"
    external/stb/stb_image.h
    src/characters/CharacterDraw.cpp)

target_include_directories(Carnival PRIVATE
    external
    external/glad/include
    external/glm
        external/miniaudio
    src/core src/game src/characters src/world
    src/ui src/states src/cutscene src/audio
)

find_package(OpenGL REQUIRED)
target_link_libraries(Carnival PRIVATE glfw OpenGL::GL freetype) #Adding freetype to target_link_libraries connects it to your game.

# Lets code find fonts/sounds from anywhere
target_compile_definitions(Carnival PRIVATE ASSET_DIR="${CMAKE_SOURCE_DIR}/assets/")

if (APPLE)
    target_compile_definitions(Carnival PRIVATE GL_SILENCE_DEPRECATION)
endif()
if (APPLE)
    target_link_libraries(Carnival PRIVATE "-framework CoreFoundation" "-framework CoreAudio" "-framework AudioToolbox")
endif()