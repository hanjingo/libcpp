# cmake .. -DBUILD_EXAMPLE=ON -DUSE_BOOST=ON

cmake_minimum_required(VERSION 3.19.2)

project(thread)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(${PROJECT_NAME}  
               main.cpp)
target_link_libraries(${PROJECT_NAME} 
               pthread)