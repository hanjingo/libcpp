cmake_minimum_required(VERSION 3.24.1)

message("libcpp/log enabled")

# set boost variant
set(BOOST_ROOT ${CMAKE_SOURCE_DIR}/3rd/boost)
set(Boost_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/3rd/boost/boost)
set(Boost_LIBRARY_DIRS ${LIBRARY_OUTPUT_PATH})
set(Boost_USE_STATIC_LIBS ON)

if (USE_BOOSTLOG)
    add_definitions(-DUSE_BOOSTLOG=ON)
    add_definitions(-DBOOST_LOG_DYN_LINK)
    find_package(Boost 1.80.0 REQUIRED COMPONENTS 
        log)
    include_directories(${Boost_INCLUDE_DIR})
endif(USE_BOOSTLOG)

if (USE_LOG4CPLUS)
    set(FETCH_LOG4CPLUS "ON" CACHE STRING INTERNAL FORCE)
    add_definitions(-DUSE_LOG4CPLUS=ON)
endif(USE_LOG4CPLUS)

if (USE_SPDLOG)
    set(FETCH_SPDLOG "ON" CACHE STRING INTERNAL FORCE)
    add_definitions(-DUSE_SPDLOG=ON)
endif(USE_SPDLOG)