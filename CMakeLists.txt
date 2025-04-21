cmake_minimum_required(VERSION 3.15)
project(MyApp LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(util_funcs)


find_package(glfw3 REQUIRED)
find_package(OpenAL REQUIRED)
find_package(assimp REQUIRED)
find_package(Freetype REQUIRED)
find_package(PNG REQUIRED)
find_package(BZip2 REQUIRED)
find_package(ZLIB REQUIRED)
find_package(SndFile REQUIRED)
find_package(glm REQUIRED)


set(RESOURCE_DIRS
    shaders
    models
    fonts
    textures
    audio
    cloud_data
)


add_subdirectory(src/core)
add_subdirectory(src/main)


if(BUILD_TESTS)
    enable_testing() 
    add_subdirectory(tests)
endif()
