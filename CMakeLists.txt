cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(jasmine-tools VERSION 1.0.0)

if (PROJECT_IS_TOP_LEVEL)
    set(GEODE_DISABLE_PRECOMPILED_HEADERS ON)
endif()

add_library(${PROJECT_NAME} STATIC src/jasmine.cpp)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

if (PROJECT_IS_TOP_LEVEL)
    add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)
    target_compile_definitions(${PROJECT_NAME} PRIVATE GEODE_MOD_ID="jasmine-tools")
endif()

target_link_libraries(${PROJECT_NAME} geode-sdk)

target_include_directories(${PROJECT_NAME} PUBLIC include)
