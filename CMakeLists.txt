cmake_minimum_required ( VERSION 2.6 )
project (selfFifxed)

include_directories("{PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/include/sg14")
include_directories("${PROJECT_SOURCE_DIR}/include")

add_compile_options (-std=c++11)
add_definitions("-Wall -g")

add_executable(Fixed test.cpp)

