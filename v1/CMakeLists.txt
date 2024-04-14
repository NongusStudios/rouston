cmake_minimum_required(VERSION 3.27)
project(demo C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)

include_directories(src/ext/glad/include)
add_subdirectory(src/ext/glfw)
add_subdirectory(src/ext/cglm)

file(GLOB SRC src/*.c src/inc/*.h)

add_executable(demo ${SRC} src/ext/impl.c src/ext/glad/src/glad.c)

target_link_libraries(demo glfw cglm)
