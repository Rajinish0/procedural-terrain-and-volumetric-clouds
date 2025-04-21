cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SOURCES
    glad.c
)

file(GLOB CPP_SOURCES CONFIGURE_DEPENDS *.cpp)
list(APPEND SOURCES ${CPP_SOURCES})

set_source_files_properties(glad.c PROPERTIES LANGUAGE C)

add_library(${PROJECT_NAME}_lib STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME}_lib PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/external
)

target_link_libraries(${PROJECT_NAME}_lib
    PUBLIC
        glfw
        OpenAL::OpenAL
        assimp::assimp
        Freetype::Freetype
        PNG::PNG
        BZip2::BZip2
        ZLIB::ZLIB
        SndFile::sndfile
	glm::glm
        gdi32
        opengl32
)

set_target_properties(${PROJECT_NAME}_lib PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
