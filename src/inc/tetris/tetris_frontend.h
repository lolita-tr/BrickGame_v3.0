/** @file */

#ifndef CPP3_S21_BrickGame2_SRC_INC_TETRIS_TETRIS_FRONTEND_H_
#define CPP3_S21_BrickGame2_SRC_INC_TETRIS_TETRIS_FRONTEND_H_

#include <ncurses.h>
#include <unistd.h>

#include "../defines.h"
#include "tetris.h"

#ifdef __cplusplus
extern "C" {
#endif

void init_ncurses();

void print_rectangle(WINDOW *win, int top_y, int bottom_y, int left_x,
                     int right_x);
void print_field(WINDOW *win);
void print_play_field(WINDOW *win, int **play_field, Tetromino *tet,
                      Game_Info *game_info);
void print_info_bar(WINDOW *win, Game_Info *game_info);

void draw_next_tetromino_in_info(WINDOW *win, Game_Info *game_info);
void draw_tetromino_on_field(WINDOW *win, Tetromino *tet);

void refresh_game(WINDOW *win, int **play_field, Tetromino *tet,
                  Game_Info *game_info);

void print_start_sceen(WINDOW *win);
void print_pause_sceen(WINDOW *win);
void print_game_over_screen(WINDOW *win, Game_Info *game_info);

void start_screen(WINDOW *startwin, Tetromino *tet, Game_Info *game_info);
void game_over_scree(WINDOW *win, Game_Info *game_info);

void user_input(Tetromino *tet, Game_Info *game_info, int sign);

#ifdef __cplusplus
}
#endif

#endif  // CPP3_S21_BrickGame2_SRC_INC_TETRIS_TETRIS_FRONTEND_H_