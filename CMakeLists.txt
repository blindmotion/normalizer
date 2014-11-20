cmake_minimum_required(VERSION 2.8.4)
project(normalizer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# add_definitions(-DPYPLOT)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)
add_executable(Normalizer ${SOURCE_FILES} ${HEADER_FILES})