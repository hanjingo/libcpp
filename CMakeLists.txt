cmake_minimum_required(VERSION 3.19.2)

project(libcpp)

# set environment param
set(LIBCPP_VERSION 1.0.0)

# build lib
if (BUILD_LIB)
    add_subdirectory(libcpp/binding)
endif()

# build example
if(BUILD_EXAMPLE)
    add_subdirectory(example)
endif()

# build tests
if (BUILD_TEST)
    add_subdirectory(tests)
endif()

install(DIRECTORY ${CMAKE_SOURCE_DIR}/libcpp
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include)