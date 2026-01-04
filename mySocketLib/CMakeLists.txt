add_library(Sockets "socket.c")

if (WIN32)
    target_link_libraries(Sockets ws2_32)
endif()

target_include_directories(Sockets PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
