cmake_minimum_required(VERSION 3.6...3.14 FATAL_ERROR)
cmake_policy(VERSION 3.6...3.14)

# load version from include/azh/version/version.hpp
file(READ "include/azh/sdk/version/version.hpp" AZH_VERSION_FILE)
string(REGEX MATCH "#define[ ]+AZH_VERSION_MAJOR[ ]+([0-9]+)" _ ${AZH_VERSION_FILE})
set(AZH_VERSION_MAJOR "${CMAKE_MATCH_1}")
string(REGEX MATCH "#define[ ]+AZH_VERSION_MINOR[ ]+([0-9]+)" _ ${AZH_VERSION_FILE})
set(AZH_VERSION_MINOR "${CMAKE_MATCH_1}")
string(REGEX MATCH "#define[ ]+AZH_VERSION_PATCH[ ]+([0-9]+)" _ ${AZH_VERSION_FILE})
set(AZH_VERSION_PATCH "${CMAKE_MATCH_1}")
string(REGEX MATCH "#define[ ]+AZH_VERSION_REVISION[ ]+([0-9]+)" _ ${AZH_VERSION_FILE})
set(AZH_VERSION_REVISION "${CMAKE_MATCH_1}")

set(AZH_VERSION ${AZH_VERSION_MAJOR}.${AZH_VERSION_MINOR}.${AZH_VERSION_PATCH})

# project version
project(azh_sdk VERSION ${AZH_VERSION} LANGUAGES CXX)
message(STATUS "azh_sdk : version " ${AZH_VERSION})

# for msvc
if(MSVC)
    add_definitions(-D _CRT_SECURE_NO_WARNINGS)
    # Specify MSVC UTF-8 encoding
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/install)

# collect sources
file(GLOB_RECURSE incs ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h*)
file(GLOB_RECURSE srcs ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*)

# library azh_sdk
add_library(azh_sdk ${incs} ${srcs})
target_include_directories(azh_sdk PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

if(AZH_ENABLE_TESTS)
    add_subdirectory(tests)
endif()

# installation configuration for azh_sdk
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

# install azh_sdk
add_custom_command(TARGET azh_sdk POST_BUILD
    COMMAND ${CMAKE_COMMAND} --install . --config $<CONFIG>
    COMMENT "Installing after build"
)