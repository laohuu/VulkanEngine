cmake_minimum_required(VERSION 3.25)
project(Editor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE HEADER_FILES "Source/*.h")
file(GLOB_RECURSE SOURCE_FILES "Source/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE Engine)
target_include_directories(${PROJECT_NAME} PUBLIC Source)

set(POST_BUILD_COMMANDS
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/Assets"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Assets" "${CMAKE_CURRENT_BINARY_DIR}/Assets"
)

add_custom_command(TARGET ${PROJECT_NAME} ${POST_BUILD_COMMANDS})

