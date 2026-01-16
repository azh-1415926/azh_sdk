cmake_minimum_required(VERSION 2.8.0)
project(logger)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} azh_sdk)