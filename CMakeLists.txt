cmake_minimum_required(VERSION 3.5)
project(hello)

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(sdl EXCLUDE_FROM_ALL)

# Create your game executable target as usual
add_executable(hello hello.c)

# Link to the actual SDL3 library.
target_link_libraries(hello PRIVATE SDL3::SDL3)
