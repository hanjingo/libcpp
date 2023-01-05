cmake_minimum_required(VERSION 3.24.1)

if (NOT DEFINED EXECUTABLE_OUTPUT_PATH)
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bins)
endif()

add_subdirectory(libcpp)

add_subdirectory(defer)

add_subdirectory(once)

# add_subdirectory(matrix)