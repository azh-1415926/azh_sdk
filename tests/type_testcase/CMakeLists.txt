cmake_minimum_required(VERSION 2.8.0)
project(type_testcase)

add_executable(${PROJECT_NAME} main.cpp type.cpp type.h)
target_link_libraries(${PROJECT_NAME} azh_sdk)