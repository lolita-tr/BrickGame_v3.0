#include "../../inc/game_common.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Prints a rectangle on a given window using ncurses extended characters.
 *
 * @param[in] win the window to draw the rectangle on
 * @param[in] top_y the y-coordinate of the top of the rectangle
 * @param[in] bottom_y the y-coordinate of the bottom of the rectangle
 * @param[in] left_x the x-coordinate of the left of the rectangle
 * @param[in] right_x the x-coordinate of the right of the rectangle
 */
void print_rectangle(WINDOW *win, int top_y, int bottom_y, int left_x, int right_x) {
  mvwaddch(win, top_y, left_x, ACS_ULCORNER);

  int i = left_x + 1;
  while (i < right_x) {
    mvwaddch(win, top_y, i++, ACS_HLINE);
  }
  mvwaddch(win, top_y, i, ACS_URCORNER);

  for (int i = top_y + 1; i < bottom_y; i++) {
    mvwaddch(win, i, left_x, ACS_VLINE);
    mvwaddch(win, i, right_x, ACS_VLINE);
  }

  mvwaddch(win, bottom_y, left_x, ACS_LLCORNER);
  i = left_x + 1;
  while (i < right_x) {
    mvwaddch(win, bottom_y, i++, ACS_HLINE);
  }
  mvwaddch(win, bottom_y, i, ACS_LRCORNER);
}

/**
 * @brief Prints the game field border.
 *
 * @param[in] win the window to draw the game field on
 */
void print_field(WINDOW *win) {
  print_rectangle(win, 0, FIELD_HEIGHT, 0, FIELD_WIDTH);
}

/**
 * @brief Prints the information bar on the game window.
 *
 * @param[in] win the window to draw the information bar on
 * @param[in] game_info the game information structure
 */
void print_info_bar(WINDOW *win, GameInfo *game_info) {
  print_rectangle(win, 0, 4, FIELD_WIDTH + 2, FIELD_WIDTH + 18);
  mvwprintw(win, 1, 19, "Level:");
  mvwprintw(win, 3, 21, "%d", game_info->level);

  print_rectangle(win, 5, 9, FIELD_WIDTH + 2, FIELD_WIDTH + 18);
  mvwprintw(win, 6, 19, "Score:");
  mvwprintw(win, 8, 16, "%6d", game_info->score);

  print_rectangle(win, 10, 14, FIELD_WIDTH + 2, FIELD_WIDTH + 18);
  mvwprintw(win, 11, 16, "High Score:");
  mvwprintw(win, 13, 16, "%6d", game_info->high_score);

  print_rectangle(win, 15, FIELD_HEIGHT, FIELD_WIDTH + 2, FIELD_WIDTH + 18);
}

/**
 * @brief Prints additional messages on the game window.
 *
 * @param[in] win the window to print the message on
 * @param[in] pause_state the current pause state of the game
 */
void print_other_message(WINDOW *win, int pause_state) {
  if (pause_state == PAUSED) {
    mvwprintw(win, 1, 1, "PAUSE");
  } else if (pause_state == WIN) {
    mvwprintw(win, 1, 1, "You won!!!");
  } else if (pause_state == LOSED) {
    mvwprintw(win, 1, 1, "You lost!");
  }
}

/**
 * @brief Gets the current high score from a file.
 *
 * @param[in] filename the path to the high score file
 * @return the high score read from the file, or 0 if file doesn't exist
 */
int get_high_score_from_file(const char* filename) {
  int high_score = 0;
  char high_score_string[100];
  FILE *file = fopen(filename, "r");

  if (file) {
    while (fgets(high_score_string, 100, file)) {
      high_score = atoi(high_score_string);
    }
    fclose(file);
  }
  return high_score;
}

/**
 * @brief Saves the current score as the high score to a file.
 *
 * @param[in] filename the path to the high score file
 * @param[in] score the score to save
 */
void save_high_score_to_file(const char* filename, int score) {
  FILE *file = fopen(filename, "w");

  if (file) {
    fprintf(file, "%d", score);
    fclose(file);
  }
}

/**
 * @brief Updates the level and speed of the game based on the score.
 *
 * @param[in] game_info the game information structure
 * @param[in] speed_step the speed step to decrease by
 */
void update_level_speed(GameInfo *game_info, int speed_step) {
  if (game_info->level < LEVEL_MAX) {
    if (game_info->score % 5 == 0) {
      ++game_info->level;
      game_info->speed -= speed_step;
    }
  }
}

/**
 * @brief Handles user input and returns the corresponding action.
 *
 * @param[in] ch the character input from the user
 * @return the corresponding UserAction
 */
UserAction handle_user_input(int ch) {
  switch (ch) {
    case KEY_UP:
      return Up;
    case KEY_DOWN:
      return Down;
    case KEY_LEFT:
      return Left;
    case KEY_RIGHT:
      return Right;
    case 'p':
    case 'P':
      return Pause;
    case 'q':
    case 'Q':
      return Terminate;
    case '\n':
      return Start;
    case ' ':
      return Action;
    default:
      return 333; // Default action
  }
} 