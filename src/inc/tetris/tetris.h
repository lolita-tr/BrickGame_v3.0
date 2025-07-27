/** @file */

#ifndef CPP3_S21_BrickGame2_SRC_INC_TETRIS_TETRIS_H_
#define CPP3_S21_BrickGame2_SRC_INC_TETRIS_TETRIS_H_

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "../defines.h"
#include "../game_common.h"

// Using common GameInfo and Coordinates from game_common.h

typedef struct {
  int type;
  int next_type;
  int **figure;
  Coordinates coord;
  bool can_spawn;
  bool is_placed;
} Tetromino;

// Using common UserAction from game_common.h instead of Signals

#ifdef __cplusplus
extern "C" {
#endif

void start_game(Tetromino *tet, GameInfo *game_info);

void start_tetris_game();
void game_update(Tetromino *tetromino, GameInfo *game_info);
void game_loop();
void free_tetromino(Tetromino *tetromino);
void free_game(GameInfo *game_info);

Tetromino *set_tetromino(GameInfo *game_info);

void set_start_position_for_tetromino(Tetromino *tetromino);
void generate_next_tetromino(Tetromino *tetromino, GameInfo *game_info,
                             int figures[7][4][4]);
void spawn_new_figure(Tetromino *tetromino, GameInfo *game_info,
                      int figures[7][4][4]);
void move_tetromino_down_one_row(Tetromino *tet, GameInfo *game_info);
void drop_tetromino(Tetromino *tet, GameInfo *game_info);
void move_tetromino_left(Tetromino *tet, GameInfo *game_info);
void move_tetromino_right(Tetromino *tet, GameInfo *game_info);
void rotate_tetromino(Tetromino *tet, GameInfo *game_info);
void terminate_game(GameInfo *game_info);

GameInfo *get_game_info();
void place_tetromino_on_field(Tetromino *tetromino, GameInfo *game_info);
int clear_line(GameInfo *game_info);
void line_dropper(GameInfo *game_info);
void check_game_over(Tetromino *tetromino, GameInfo *game_info);

void score_update(GameInfo *game_info, int counter);
void level_speed_update(GameInfo *game_info);
void save_high_score(GameInfo *game_info);
int get_high_score();
int generate_figure();
void pause_game(GameInfo *game_info);
void stat_matrix_to_dyn(int **dest, int src[MAX_FIGURE_SIZE][MAX_FIGURE_SIZE]);

#ifdef __cplusplus
}
#endif

#endif  // CPP3_S21_BrickGame2_SRC_INC_TETRIS_TETRIS_H_