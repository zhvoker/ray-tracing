cmake_minimum_required (VERSION 3.25)

project (ray-tracing)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

aux_source_directory (src SRC_LIST)

include_directories (includes)

add_compile_options(-std=c++11 -Wall)

add_executable (main ${SRC_LIST})