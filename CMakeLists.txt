cmake_minimum_required(VERSION 4.3)
project(metronome)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

add_subdirectory(vendored/SDL_ttf EXCLUDE_FROM_ALL)

add_subdirectory(vendored/SDL_mixer EXCLUDE_FROM_ALL)

add_executable(metronome MACOSX_BUNDLE main.c metronome.c player.c button.c)
# Link to the actual SDL3 library.
target_link_libraries(metronome PRIVATE SDL3_mixer::SDL3_mixer SDL3_ttf::SDL3_ttf SDL3::SDL3)
