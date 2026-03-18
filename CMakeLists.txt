cmake_minimum_required(VERSION 3.11)
project(list LANGUAGES C)

# list_core ---------------------------

add_library(list_core
    src/list.c
)

target_include_directories(list_core
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

# list --------------------------------

add_executable(list
    src/main.c
)

target_include_directories(list
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(list
    PRIVATE
        list_core
)

#---------------------------------------
