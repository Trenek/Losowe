cmake_minimum_required(VERSION 3.30)
project(Losowy)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

add_subdirectory(mySocketLib)

add_executable(Losowy 
    "${CMAKE_CURRENT_SOURCE_DIR}/source/main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/page.c"
)

target_link_libraries(Losowy Sockets)
target_include_directories(Losowy PRIVATE 
    Sockets
    "${CMAKE_CURRENT_SOURCE_DIR}/source/"
)

set(PAGE_SRC "${CMAKE_SOURCE_DIR}/page")
set(PAGE_DST "${CMAKE_BINARY_DIR}/page")

add_custom_target(pages DEPENDS ${PAGE_SRC})

add_custom_command(TARGET pages POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PAGE_SRC} ${PAGE_DST}
)

add_dependencies(Losowy pages)
