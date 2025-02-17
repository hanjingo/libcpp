cmake_minimum_required(VERSION 3.19.2)

# cmake -DBUILD_TEST=ON ..
# make
# make test

project(tests)

# set binary output path
if (NOT EXECUTABLE_OUTPUT_PATH)
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
endif()

enable_testing()

find_package(GTest REQUIRED)
find_package(ZeroMQ REQUIRED)
# find_package(SQLite3 REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(httplib REQUIRED)
# find_package(BehaviorTreeV3 REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(pugixml REQUIRED)
find_package(Boost REQUIRED 
    system 
    filesystem 
    context 
    thread 
    coroutine)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${GTEST_INCLUDE_DIR})
include_directories(${ZeroMQ_INCLUDE_DIR})
# include_directories(${SQLite3_INCLUDE_DIRS})
include_directories(${Eigen_INCLUDE_DIRS})
include_directories(${OpenSSL_INCLUDE_DIRS})
include_directories(${PUGIXML_INCLUDE_DIR})

if (NOT Breakpad_INCLUDE_DIR)
    set(Breakpad_INCLUDE_DIR /usr/local/include/breakpad)
endif()
include_directories(${Breakpad_INCLUDE_DIR})

aux_source_directory(. SRC)

add_executable(${PROJECT_NAME} ${SRC})

if (NOT Breakpad_LIB_DIR)
    set(Breakpad_LIB_DIR /usr/local/lib)
endif()
link_directories(${Breakpad_LIB_DIR})

target_link_libraries(${PROJECT_NAME} 
    ${GTEST_LIBRARIES} 
    ${Boost_LIBRARIES} 
    # ${SQLite3_LIBRARIES} 
    ${Eigen_LIBRARIES} 
    pthread
    pugixml
    # BT::behaviortree_cpp_v3
    OpenSSL::SSL 
    OpenSSL::Crypto
    pugixml
    breakpad_client
)

add_test(test ${PROJECT_NAME})