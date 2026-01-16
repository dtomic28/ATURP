cmake_minimum_required(VERSION 3.10)

set(ASSIGNMENT_NAME "seminarska")

project(${ASSIGNMENT_NAME})

include_directories(include)

set(SOURCES
        src/main.cpp
        # Add more source files here as needed
)

add_executable(${ASSIGNMENT_NAME} ${SOURCES})

set(SOURCE_ASSETS_DIR "${CMAKE_SOURCE_DIR}/data")

set(DEST_ASSETS_DIR "${CMAKE_BINARY_DIR}")

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SOURCE_ASSETS_DIR}" "${DEST_ASSETS_DIR}"
        COMMENT "Copying assets to build directory..."
)

