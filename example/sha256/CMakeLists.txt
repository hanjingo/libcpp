cmake_minimum_required(VERSION 3.19.2)

project(sha256)

find_package(OpenSSL)

if (OpenSSL_FOUND)
include_directories(${CMAKE_SOURCE_DIR})

add_executable(${PROJECT_NAME} 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(${PROJECT_NAME} OpenSSL::SSL)

else()
message("!!! openssl not found, skip build example/sha256")

endif()