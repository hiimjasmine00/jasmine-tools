cmake_minimum_required(VERSION 3.21)

project(jasmine-tools VERSION 1.2.0)

add_library(jasmine-tools INTERFACE)

target_sources(jasmine-tools INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src/jasmine.cpp)

target_compile_features(jasmine-tools INTERFACE cxx_std_23)
set_target_properties(jasmine-tools PROPERTIES CXX_VISIBILITY_PRESET hidden)

target_include_directories(jasmine-tools INTERFACE include)

if (NOT TARGET fast_float)
    if (NOT COMMAND CPMAddPackage)
        include(cmake/get_cpm.cmake)
    endif()

    CPMAddPackage("gh:fastfloat/fast_float@8.2.1")
    target_link_libraries(jasmine-tools INTERFACE fast_float)
endif()

if (PROJECT_IS_TOP_LEVEL)
    add_subdirectory(test)
endif()
