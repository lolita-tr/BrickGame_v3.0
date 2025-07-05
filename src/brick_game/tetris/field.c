#include "../../inc/tetris/figures.h"
#include "../../inc/tetris/tetris.h"

/** @file */

/**
 * @brief Updates the game state and handles all game logic.
 * @details This function updates the game state by spawning a new tetromino,
 * clearing any full lines, dropping any lines above the cleared ones, updating
 * the game score, and then updating the game speed based on the current level.
 *
 * @param tetromino - pointer to the Tetromino structure
 * @param game_info - pointer to the Game_Info structure
 */
void game_update(Tetromino *tetromino, Game_Info *game_info) {
  spawn_new_figure(tetromino, game_info, figures);
  int cleared = clear_line(game_info);
  line_dropper(game_info);
  score_update(game_info, cleared);
  level_speed_update(game_info);
}
/**
 * @brief Get a pointer to the game information structure
 * @details This function returns a pointer to the game information structure.
 *          It is a singleton, so it will only allocate memory for the structure
 *          on the first call and return the same pointer on subsequent calls.
 * @return A pointer to the game information structure
 */
Game_Info *get_game_info() {  // изменила
  Game_Info *game_info = calloc(1, sizeof(Game_Info));

  game_info->field = calloc(FIELD_H, sizeof(int **));
  for (int i = 0; i < FIELD_H; i++) {
    game_info->field[i] = calloc(FIELD_W, sizeof(int));
  }

  game_info->next = calloc(MAX_FIGURE_SIZE, sizeof(int *));
  for (int i = 0; i < MAX_FIGURE_SIZE; i++) {
    game_info->next[i] = calloc(MAX_FIGURE_SIZE, sizeof(int));
  }

  game_info->high_score = get_high_score();
  game_info->level = LEVEL_MIN;
  game_info->speed = SPEED_1;
  game_info->pause = NOT_STARTED;
  return game_info;
}

/**
 * @brief Place a tetromino on the game field
 * @details This function takes a tetromino and the game information structure
 * as parameters. It checks if the tetromino is placed and if it is, it places
 * the tetromino on the game field. It loops over the figure and checks if the
 * given cell is a part of the tetromino (i.e., if the cell is not empty). If it
 * is, it places the cell on the game field by setting the corresponding cell in
 * the game information structure to 1.
 * @param tetromino A pointer to the tetromino structure
 * @param game_info A pointer to the game information structure
 */
void place_tetromino_on_field(Tetromino *tetromino, Game_Info *game_info) {
  if (tetromino->is_placed) {
    for (int y = 0; y < MAX_FIGURE_SIZE; y++) {
      for (int x = 0; x < MAX_FIGURE_SIZE; x++) {
        if (tetromino->figure[y][x]) {
          int y_field = tetromino->coord.y + y;
          int x_field = tetromino->coord.x + x;

          game_info->field[y_field][x_field] = 1;
        }
      }
    }
  }
}

/**
 * @brief Clears full lines on the game field
 * @details This function takes a pointer to the game information structure as a
 *          parameter. It loops over the game field and checks for each row
 *          if it is full (i.e., if none of the cells are empty). If a row is
 *          full, it clears the row by setting all cells to 0. The function
 *          returns the number of cleared rows.
 * @param game_info A pointer to the game information structure
 * @return The number of cleared rows
 */
int clear_line(Game_Info *game_info) {
  int line_counter = 0;
  for (int y = 0; y < FIELD_H; y++) {
    int line_incomplete = 0;
    for (int x = 0; x < FIELD_W && !line_incomplete; x++) {
      if (!game_info->field[y][x]) {
        line_incomplete = 1;
      }
    }
    if (!line_incomplete) {
      for (int x = 0; x < FIELD_W; x++) {
        game_info->field[y][x] = 0;
      }
      line_counter++;
    }
  }
  return line_counter;
}

/**
 * @brief Drops lines in the game field when a row is cleared.
 * @details This function iterates from the bottom of the game field to the top,
 *          checking each row to see if it is empty. If a row is empty, it
 * shifts all rows above it down by one. The operation continues until 100 line
 *          drops have been performed or the top of the field is reached.
 * @param game_info A pointer to the game information structure.
 */
void line_dropper(Game_Info *game_info) {
  int counter = 100;
  for (int y = FIELD_H - 1; y > 0 && counter; y--) {
    int line_clear = 1;
    for (int x = 0; x < FIELD_W && line_clear; x++) {
      if (game_info->field[y][x]) {
        line_clear = 0;
      }
    }
    if (line_clear) {
      for (int k = y; k > 0 && counter; k--) {
        for (int x = 0; x < FIELD_W; x++) {
          game_info->field[k][x] = game_info->field[k - 1][x];
        }

        counter--;
      }
      y++;
    }
  }
}

/**
 * @brief Checks if the game is over by determining if the current tetromino can
 * spawn.
 * @details This function iterates over the tetromino's figure and checks if any
 * part of it overlaps with filled cells in the game field. If an overlap is
 * detected, it sets the tetromino's can_spawn attribute to FALSE, indicating
 * that the game is over.
 * @param tetromino A pointer to the tetromino structure.
 * @param game_info A pointer to the game information structure.
 */
void check_game_over(Tetromino *tetromino, Game_Info *game_info) {
  for (int y = 1; y < MAX_FIGURE_SIZE; y++) {
    for (int x = 0; x < MAX_FIGURE_SIZE; x++) {
      if (game_info->field[tetromino->coord.y + y][tetromino->coord.x + x] &&
          tetromino->figure[y][x]) {
        tetromino->can_spawn = FALSE;
        game_info->pause = LOSED;
      }
    }
  }
}