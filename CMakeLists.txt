cmake_minimum_required(VERSION 3.28)
project(ring_buffer_mcu)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(cmake/get_cpm.cmake)
include(cmake/CPM.cmake)

CPMAddPackage("gh:nboutin/buffer_mcu@1.1.0")

if(RING_BUFFER_MCU_TEST)
    include(cmake/test_config.cmake)
endif()

add_library(${PROJECT_NAME} OBJECT
  source/ring_buffer.c
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    include)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    buffer_mcu
  PRIVATE
)

if(RING_BUFFER_MCU_TEST)
  add_subdirectory(test)
endif()
