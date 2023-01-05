cmake_minimum_required(VERSION 3.24.1)

# cmake .. -DBUILD_TEST=ON -DBOOST=ON -DTENSORFLOW=ON -DBEHAVIOR_TREE=ON -DCONCURRENTQUEUE=ON -DOPENSSL=ON -DNLOHMANN_JSON=ON -DRAPIDJSON=ON -DPUGIXML=ON -DOPENCV=ON -DLIBZMQ=ON -DZMQPP=ON -DLOG4CPLUS=ON -DLIBHARU=ON -DHIREDIS=ON

# set environment param
set(CMAKE_CXX_STANDARD 11)

# set project info
project(libcpp)

set(LIBCPP_VERSION 0.0.1)

# import libcpp
add_subdirectory(libcpp)

# import 3rd component
add_subdirectory(3rd)

# example
if(BUILD_TEST)
    add_subdirectory(example)
endif()

