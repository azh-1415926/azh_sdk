cmake_minimum_required(VERSION 3.6...3.14 FATAL_ERROR)
cmake_policy(VERSION 3.6...3.14)

# 从 version.hpp 中获取版本信息
file(READ "include/azh/version/version.hpp" AZH_VERSION_FILE)
string(REGEX MATCH "#define[ ]+AZH_VERSION_MAJOR[ ]+([0-9]+)" _ ${AZH_VERSION_FILE})
set(AZH_VERSION_MAJOR "${CMAKE_MATCH_1}")
string(REGEX MATCH "#define[ ]+AZH_VERSION_MINOR[ ]+([0-9]+)" _ ${AZH_VERSION_FILE})
set(AZH_VERSION_MINOR "${CMAKE_MATCH_1}")
string(REGEX MATCH "#define[ ]+AZH_VERSION_PATCH[ ]+([0-9]+)" _ ${AZH_VERSION_FILE})
set(AZH_VERSION_PATCH "${CMAKE_MATCH_1}")
string(REGEX MATCH "#define[ ]+AZH_VERSION_REVISION[ ]+([0-9]+)" _ ${AZH_VERSION_FILE})
set(AZH_VERSION_REVISION "${CMAKE_MATCH_1}")

set(AZH_VERSION ${AZH_VERSION_MAJOR}.${AZH_VERSION_MINOR}.${AZH_VERSION_PATCH})

# 设置项目版本
project(azh_sdk VERSION ${AZH_VERSION} LANGUAGES CXX)
message(STATUS "azh_sdk : version " ${AZH_VERSION})

if(MSVC)
    add_definitions(-D _CRT_SECURE_NO_WARNINGS)
endif()

set(CMAKE_CXX_STANDARD 17)

find_package(CURL REQUIRED)
find_package(unofficial-minizip REQUIRED)

file(GLOB_RECURSE incs ${CMAKE_SOURCE_DIR}/include/*.h*)
file(GLOB_RECURSE srcs ${CMAKE_SOURCE_DIR}/src/*.c*)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

add_library(azh_sdk ${incs} ${srcs})

target_include_directories(azh_sdk PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

target_link_libraries(azh_sdk PUBLIC
    CURL::libcurl
    unofficial::minizip::minizip
)

install(
	DIRECTORY include
	DESTINATION "."
    # PATTERN "CMakeLists.txt" EXCLUDE    
)

install(TARGETS azh_sdk
    EXPORT azh_sdk-targets
    PUBLIC_HEADER DESTINATION include
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)
install(EXPORT azh_sdk-targets
    NAMESPACE azh_sdk::
    FILE azh_sdk-Target.cmake
    DESTINATION lib/cmake/azh_sdk)

include(CMakePackageConfigHelpers)
configure_package_config_file(
        ${PROJECT_SOURCE_DIR}/cmake/azh_sdk-Config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/azh_sdk-Config.cmake
        INSTALL_DESTINATION lib/cmake
        )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/azh_sdk-Config.cmake DESTINATION lib/cmake/azh_sdk)

add_subdirectory(tests)