cmake_minimum_required(VERSION 3.10)
project(BrickGame_v3.0 CXX)

add_executable(my_executable brick_game/snake/snake.cpp
                brick_game/snake/snake_controller.cpp
                brick_game/snake/snake_view.cpp

                brick_game/tetris/field.c
                brick_game/tetris/figure.c
                brick_game/tetris/fsm.c
                brick_game/tetris/utility.c
)