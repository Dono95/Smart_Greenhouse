cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 11)

set(EXTRA_COMPONENT_DIRS Drivers)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

#add_compile_definitions(TEST_FLAG)

project(SmartGreenhouse)
