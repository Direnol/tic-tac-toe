cmake_minimum_required(VERSION 3.8)
project(tic_tac_toe)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES_CLIENT
        Client/client.cpp Client/ClientChat.cpp Client/ClientChat.h
        Headers/Common.h Interface/Gui.cpp Interface/Gui.h)


set(SOURCE_FILES_SERVER
        Server/server.cpp Server/ServerChat.cpp Server/ServerChat.h Headers/Common.h)

add_executable(server ${SOURCE_FILES_SERVER})
target_link_libraries(server pthread ncurses)

add_executable(client ${SOURCE_FILES_CLIENT})
target_link_libraries(client pthread ncurses)