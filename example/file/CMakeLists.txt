cmake_minimum_required(VERSION 3.19.2)

project(file)

find_package(Boost COMPONENTS 
    filesystem)

if(Boost_FOUND)
include_directories(${CMAKE_SOURCE_DIR})

add_executable(${PROJECT_NAME}  
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

else()
message("!!! boost not found, skip build example/file")

endif()