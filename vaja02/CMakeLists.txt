cmake_minimum_required(VERSION 3.10)

set(ASSIGNMENT_NAME "vaja02")

project(${ASSIGNMENT_NAME})

include_directories(${CMAKE_SOURCE_DIR}/${ASSIGNMENT_NAME}/include)

set(SOURCES
    src/main.cpp
    # Add more source files here as needed
)

add_executable(${ASSIGNMENT_NAME} ${SOURCES})

set(SOURCE_ASSETS_DIR "${CMAKE_SOURCE_DIR}/${ASSIGNMENT_NAME}/data")

# Path to the destination folder inside the build directory
set(DEST_ASSETS_DIR "${CMAKE_BINARY_DIR}/${ASSIGNMENT_NAME}")

# Add a custom post-build command
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SOURCE_ASSETS_DIR}" "${DEST_ASSETS_DIR}"
        COMMENT "Copying assets to build directory..."
)
