cmake_minimum_required(VERSION 3.24.1)

message("libcpp/math enabled")

if (USE_OPENCV_MAT)
    add_definitions(-DUSE_OPENCV_MAT=ON)
endif(USE_OPENCV_MAT)