/** @file */

#ifndef CPP3_S21_BrickGame2_SRC_INC_GAME_COMMON_H_
#define CPP3_S21_BrickGame2_SRC_INC_GAME_COMMON_H_

#include <ncurses.h>
#include "defines.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Common game information structure for both Snake and Tetris
 */
typedef struct {
  int** field;        // Main game field
  int** next;         // Next piece info (for Tetris) or apple (for Snake)
  int score;          // Current score
  int high_score;     // High score
  int level;          // Current level
  int speed;          // Game speed
  int pause;          // Game state (NOT_STARTED, STARTED, PAUSED, etc.)
} GameInfo;

/**
 * @brief Common user actions for both games
 */
typedef enum {
  Start,
  Pause,
  Terminate,
  Action,
  Left,
  Right,
  Up,
  Down
} UserAction;

/**
 * @brief Common coordinates structure
 */
typedef struct {
  int x;
  int y;
} Coordinates;

// Common UI functions
void print_rectangle(WINDOW *win, int top_y, int bottom_y, int left_x, int right_x);
void print_field(WINDOW *win);
void print_info_bar(WINDOW *win, GameInfo *game_info);
void print_other_message(WINDOW *win, int pause_state);

// Common game utility functions
int get_high_score_from_file(const char* filename);
void save_high_score_to_file(const char* filename, int score);
void update_level_speed(GameInfo *game_info, int speed_step);

// Common input handling
UserAction handle_user_input(int ch);

#ifdef __cplusplus
}
#endif

#endif  // CPP3_S21_BrickGame2_SRC_INC_GAME_COMMON_H_ 