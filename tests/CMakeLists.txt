# find_package(GTest REQUIRED)
# set(PROJ_NAME ${PROJECT_NAME})
# project(${PROJECT_NAME}_tests LANGUAGES C CXX)

# file(GLOB TEST_SOURCES CONFIGURE_DEPENDS *.cpp)

# add_executable(${PROJECT_NAME} ${TEST_SOURCES})

# target_include_directories(${PROJECT_NAME} PRIVATE 
#     ${CMAKE_SOURCE_DIR}/include 
#     ${CMAKE_SOURCE_DIR}/include/external
#     include
# )

# target_link_libraries(${PROJECT_NAME}
#     PRIVATE
#         ${PROJ_NAME}_lib
#         GTest::gtest
# 	#GTest::gtest_main
# 	GTest::gmock_main
# )

# add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

# #set_target_properties(${PROJECT_NAME}_tests PROPERTIES
# #    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
# #)

# #include(GoogleTest)
# #gtest_discover_tests(${PROJECT_NAME})

## FOR SOME REASON when i link with GTest::gmock VS cannot discover the tests
## it might be due to it not being able to find the debug symbols for it
## this way below is slower but it works

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG cd430b47a54841ec45d64d2377d7cabaf0eba610
#   URL https://github.com/google/googletest/archive/refs/heads/main.zip
#  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

set(PROJ_NAME ${PROJECT_NAME})
project(${PROJECT_NAME}_tests LANGUAGES C CXX)

file(GLOB TEST_SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable(${PROJECT_NAME} ${TEST_SOURCES})

set(CMAKE_DEBUG_POSTFIX "d")

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/external
    include
)


target_link_libraries(${PROJECT_NAME} PRIVATE
    ${PROJ_NAME}_lib
    GTest::gtest
    GTest::gmock
    GTest::gtest_main
)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

message(${PROJECT_SOURCE_DIR})

add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${PROJECT_SOURCE_DIR}/resources
	${CMAKE_CURRENT_BINARY_DIR}/resources
)

# Optionally, if you need to enable test discovery, uncomment the following:
# set_target_properties(${PROJECT_NAME} PROPERTIES ENABLE_TEST_DISCOVERY TRUE)
