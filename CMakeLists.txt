cmake_minimum_required(VERSION 3.28)
project(ring_buffer_mcu)

add_library(${PROJECT_NAME} OBJECT
  source/ring_buffer.c
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    include)

include(cmake/get_cpm.cmake)
include(cmake/CPM.cmake)

CPMAddPackage("gh:nboutin/buffer_mcu@1.0.0")

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    buffer_mcu
  PRIVATE
)

add_subdirectory(test)
