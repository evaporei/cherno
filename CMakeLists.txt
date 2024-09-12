cmake_minimum_required(VERSION 3.22)
project(Cherno VERSION 1.0 LANGUAGES CXX)

# Enable Clangd LSP integration with the project
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ------------------------------------------------------------------------------
# Third-party dependencies
# ------------------------------------------------------------------------------

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(THIRDPARTY_DIR "${CMAKE_SOURCE_DIR}/thirdparty")

add_subdirectory("${THIRDPARTY_DIR}/glfw")
add_subdirectory("${THIRDPARTY_DIR}/glad")
add_subdirectory("${THIRDPARTY_DIR}/glm")

# ------------------------------------------------------------------------------
# Source files
# ------------------------------------------------------------------------------

set(MY_FLAGS "-std=c++20" "-Wall")
set(SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_SOURCE_DIR}/src/index_buffer.cpp"
    "${CMAKE_SOURCE_DIR}/src/renderer.cpp"
    "${CMAKE_SOURCE_DIR}/src/shader.cpp"
    "${CMAKE_SOURCE_DIR}/src/texture.cpp"
    "${CMAKE_SOURCE_DIR}/src/vertex_array.cpp"
    "${CMAKE_SOURCE_DIR}/src/vertex_buffer.cpp"
    "${CMAKE_SOURCE_DIR}/src/vertex_buffer_element.cpp"
    "${CMAKE_SOURCE_DIR}/src/vertex_buffer_layout.cpp"
)

add_executable(cherno_exe ${SOURCE_FILES})
target_compile_options(cherno_exe PUBLIC ${MY_FLAGS})

add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE thirdparty)

target_link_libraries(cherno_exe PUBLIC glfw glm::glm glad stb_image)
