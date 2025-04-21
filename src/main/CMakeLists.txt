cmake_minimum_required(VERSION 3.15)

file(GLOB SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
PRIVATE
	${PROJECT_NAME}_lib
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

copy_resources_to_current_bin_dir()

