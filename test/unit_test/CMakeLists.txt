cmake_minimum_required(VERSION 3.28)
project(ring_buffer_mcu_ut)

add_executable(${PROJECT_NAME}
  $<TARGET_OBJECTS:ring_buffer_mcu>
  $<TARGET_OBJECTS:buffer_mcu>
  suites/ut_rbuf_get_free_size.cpp
  suites/ut_rbuf_get_used_size.cpp
  suites/ut_rbuf_init.cpp
  suites/ut_rbuf_is_empty.cpp
  suites/ut_rbuf_is_full.cpp
  suites/ut_rbuf_read_copy_block.cpp
  suites/ut_rbuf_read_copy_raw.cpp
  suites/ut_rbuf_read_uint8.cpp
  suites/ut_rbuf_write_copy.cpp
  suites/ut_rbuf_write_string.cpp
  suites/ut_rbuf_write_uint8.cpp
  suites/ut_rbuf.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE ring_buffer_mcu gtest gtest_main gmock)
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
