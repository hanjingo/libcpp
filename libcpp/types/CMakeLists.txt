cmake_minimum_required(VERSION 3.24.1)

message("libcpp/types enabled")

if (USE_BOOST_TYPES)
    add_definitions(-DUSE_BOOST_TYPES=ON)
endif(USE_BOOST_TYPES)