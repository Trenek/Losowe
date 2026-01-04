cmake_minimum_required(VERSION 3.30)
project(Losowy)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

add_executable(Losowy "socket.c")

if (WIN32)
    target_link_libraries(Losowy ws2_32)
endif()

set(PAGE_SRC "${CMAKE_SOURCE_DIR}/page")
set(PAGE_DST "${CMAKE_BINARY_DIR}/page")

add_custom_target(pages DEPENDS ${PAGE_SRC})

add_custom_command(TARGET pages POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PAGE_SRC} ${PAGE_DST}
)

add_dependencies(Losowy pages)
