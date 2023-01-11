cmake_minimum_required(VERSION 3.24.1)

# set environment param
set(CMAKE_CXX_STANDARD 11)
set(LIBCPP_VERSION 0.0.1)

# option
## algo option
option(ALGORITHM "enable algorithm" ON)
## binding option
option(BINDING "enable binding" OFF)
## container option
option(CONTAINER "enable container" ON)
## crypto option
option(CRYPTO "enable crypto" ON)
## db option
option(DB "enable db" ON)
## encoding
option(ENCODING "enable encoding" ON)
## log option
option(LOG "enable log" ON)
option(USE_BOOSTLOG "use boost log" ON)
option(USE_LOG4CPLUS "use log4cplus" OFF)
option(USE_SPDLOG "use spdlog" OFF)
## env option
option(ENV "enable env" ON)
## file option
option(FILE "enable file" ON)
## graphical option
option(GRAPHICAL "enable graphical" ON)
## io option
option(IO "enable io" ON)
option(USE_BOOST_IO "use boost io" OFF)
## math option
option(MATH "enable math" ON)
option(USE_OPENCV_MAT "use opencv matrix" OFF)
## memory option
option(MEMORY "enable memory" ON)
## meta option
option(META "enable meta" ON)
## misc option
option(MISC "enable misc" ON)
## net option
option(NET "enable net" ON)
## os option
option(OS "enable os" ON)
## plugin option
option(PLUGIN "enable plugin" ON)
## strings option
option(STRINGS "enable strings" ON)
## sync option
option(SYNC "enable sync" ON)
## test option
option(TEST "enable test" ON)
## time option
option(TIME "enable time" ON)
## types option
option(TYPES "enable types" ON)
option(USE_BOOST_TYPES "use boost types" OFF)
## util option
option(UTIL "enable util" ON)

# import 3rd component
add_subdirectory(3rd)

# import libcpp
add_subdirectory(libcpp)

# example
if(BUILD_TEST)
    add_subdirectory(example)
endif()
