cmake_minimum_required(VERSION 2.8.8)
set(PROJECT_NAME_STR fixed_point)

project(${PROJECT_NAME_STR} C CXX)

include_directories("${DEPS_ROOT}/include")
link_directories("${DEPS_ROOT}/lib")
link_directories("${DEPS_ROOT}/lib64")

set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(${COMMON_INCLUDES})

set (CMAKE_CXX_STANDARD 11)
find_package (PythonLibs REQUIRED)
find_package (PythonInterp REQUIRED)

include_directories ("${PYTHON_INCLUDE_DIRS}")
message(STATUS "PYTHON_INCLUDE_DIRS" ${PYTHON_INCLUDE_DIRS})

add_library (fix32 SHARED
	"${CMAKE_SOURCE_DIR}/src/FixedPoint.cpp"
)

set_target_properties(fix32 PROPERTIES PREFIX "")
set_target_properties(fix32 PROPERTIES OUTPUT_NAME "fix32")

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -ansi -Wno-deprecated -std=c++0x")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -ansi -Wno-deprecated -std=c++0x")
	set_target_properties (fix32 PROPERTIES SUFFIX ".so")
endif()

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_VARIADIC_MAX=10")
	set_target_properties (fix32 PROPERTIES SUFFIX ".pyd")
endif() 

if(WIN32)
    set(_OPT_CMAKE_ARGS "-Dgtest_force_shared_crt=ON;-DCMAKE_SH=CMAKE_SH-NOTFOUND")
	set_target_properties (fix32 PROPERTIES SUFFIX ".pyd")
else()
    set(_OPT_CMAKE_ARGS "")
endif()

target_link_libraries (fix32 "${PYTHON_LIBRARIES}")

