cmake_minimum_required(VERSION 3.8)
project(tic_tac_toe)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES_CLIENT
        client.cpp ClientChat.cpp ClientChat.h Common.h)


set(SOURCE_FILES_SERVER
        server.cpp ServerChat.cpp ServerChat.h Common.h)

add_executable(${PROJECT_NAME}_server ${SOURCE_FILES_SERVER})
target_link_libraries(${PROJECT_NAME}_server pthread ncurses)

add_executable(${PROJECT_NAME}_client ${SOURCE_FILES_CLIENT})
target_link_libraries(${PROJECT_NAME}_client pthread ncurses)