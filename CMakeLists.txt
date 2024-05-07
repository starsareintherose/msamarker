cmake_minimum_required(VERSION 3.0)

project(tnt_script)
SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )

set(CMAKE_CXX_STANDARD 11)

add_executable(gene_marker gene_marker.cpp)
add_executable(trim_marker trim_marker.cpp)
	
target_compile_options(gene_marker PRIVATE -Wall -Wextra -pedantic -O3)
target_compile_options(trim_marker PRIVATE -Wall -Wextra -pedantic -O3)
