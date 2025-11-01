cmake_minimum_required(VERSION 3.21)

project(jasmine-tools VERSION 1.0.0)

add_library(jasmine-tools INTERFACE)

target_sources(jasmine-tools INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src/jasmine.cpp)

target_compile_features(jasmine-tools INTERFACE cxx_std_23)
set_target_properties(jasmine-tools PROPERTIES CXX_VISIBILITY_PRESET hidden)

target_include_directories(jasmine-tools INTERFACE include)

if (PROJECT_IS_TOP_LEVEL)
    add_subdirectory(test)
endif()
