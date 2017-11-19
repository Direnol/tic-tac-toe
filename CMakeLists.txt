cmake_minimum_required(VERSION 3.9)
project(tic_tac_toe)

set(CMAKE_CXX_STANDARD 17)



set(SOURCE_FILES_SERVER
        main.cpp ServerChat.cpp ServerChat.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILES_SERVER})
target_link_libraries(${PROJECT_NAME} pthread ncurses)