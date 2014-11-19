set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# add_definitions(-DPYPLOT)

set(SOURCE_FILES main.cpp plot.cpp utils.cpp config.cpp)

set(HEADER_FILES plot.hpp types.hpp utils.hpp config.hpp)

add_executable(Normalizer ${SOURCE_FILES} ${HEADER_FILES})