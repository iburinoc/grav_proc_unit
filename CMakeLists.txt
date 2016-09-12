cmake_minimum_required(VERSION 2.6)
project(gpu)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

find_package(OpenCL)
find_package(OpenGL)
find_package(GLFW)
find_package(GLEW)

include_directories(
	${GLFW_INCLUDE_DIR}
	${OpenGL_INCLUDE_DIR}
	${GLEW_INCLUDE_DIRS}
	${OpenCL_INCLUDE_DIRS}
	${gpu_SOURCE_DIR}/inc
)

message("include directories:")
get_directory_property(includes INCLUDE_DIRECTORIES)
message("${includes}\n")

add_definitions(-g)
set(CMAKE_CXX_STANDARD 11)

# convenient to have main in src/ folder, don't want to compile to lib though
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB MAIN_CPP src/main.cpp)
list(REMOVE_ITEM SOURCES ${MAIN_CPP})

file(GLOB_RECURSE HEADERS src/*.hpp)

message("sources:")
message("${SOURCES}\n")

add_library(gpu_lib ${SOURCES})
target_compile_features(gpu_lib PRIVATE cxx_raw_string_literals)

set(MAIN_EXEC gpu)
add_executable(${MAIN_EXEC} ${MAIN_CPP})
target_link_libraries(${MAIN_EXEC}
	gpu_lib
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARY}
	${GLFW_LIBRARIES}
	${OpenCL_LIBRARIES})

file(GLOB TEST_SOURCES test/*.cpp)
foreach(test_source ${TEST_SOURCES})
	get_filename_component(test_name ${test_source} NAME_WE)
	string(CONCAT test_name "test_" ${test_name})
	message("test script ${test_name}: ${test_source}\n")
	add_executable(${test_name} ${test_source})
	target_link_libraries(${test_name}
		gpu_lib
		${OPENGL_LIBRARIES}
		${GLEW_LIBRARY}
		${GLFW_LIBRARIES}
		${OpenCL_LIBRARIES})
endforeach(test_source ${TEST_SOURCES})

