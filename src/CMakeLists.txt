add_subdirectory(gflags)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/plot.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/config.cpp
    PARENT_SCOPE
)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/plot.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/types.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/config.hpp
    PARENT_SCOPE
)
