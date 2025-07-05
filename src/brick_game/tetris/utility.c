#include "../../inc/tetris/figures.h"
#include "../../inc/tetris/tetris.h"
/** @file */

/**
 * @brief Updates the score of a game and the high score if necessary.
 *
 * Updates the score of a game based on the number of cleared lines in a single
 * move. If the score is higher than the current high score, the high score will
 * be updated and saved.
 *
 * @param game_info The game information
 * @param counter The number of cleared lines
 */
void score_update(Game_Info *game_info, int counter) {
  if (counter == 1) {
    game_info->score += 100;
  } else if (counter == 2) {
    game_info->score += 300;
  } else if (counter == 3) {
    game_info->score += 700;
  } else if (counter == 4) {
    game_info->score += 1500;
  }

  if (game_info->score > game_info->high_score) {
    game_info->high_score = game_info->score;
    save_high_score(game_info);
  }
}

/**
 * @brief Updates the level and speed of the game based on the score.
 *
 * This function calculates the current level by dividing the score by 600.
 * It then updates the game's speed by subtracting 1500 times the level
 * from the initial speed (SPEED_1). The level will only increase if it is
 * below LEVEL_MAX.
 *
 * @param game_info The game information structure containing score, level, and
 * speed
 */
void level_speed_update(Game_Info *game_info) {
  if (game_info->level < LEVEL_MAX) {
    game_info->level = game_info->score / 600;

    game_info->speed = SPEED_1 - game_info->level * 3000;
  }
}

/**
 * @brief Saves the current high score to a file.
 *
 * This function writes the high score from the game information
 * structure to a file specified by HIGH_SCORE_PATH. If the file
 * can be successfully opened, the high score is written to it
 * and the file is then closed.
 *
 * @param game_info The game information structure containing the high score
 */
void save_high_score(Game_Info *game_info) {
  int high_score = game_info->high_score;
  FILE *file = fopen(HIGH_SCORE_PATH, "w");

  if (file) {
    fprintf(file, "%d", high_score);
    fclose(file);
  }
}

/**
 * @brief Gets the current high score from a file.
 *
 * This function reads the high score from a file specified by HIGH_SCORE_PATH.
 * It reads the file line by line and converts each line to an integer, storing
 * the last value read in the high_score variable. The file is then closed and
 * the high score is returned.
 *
 * @return The high score read from the file
 */
int get_high_score() {
  int high_score = 0;
  char high_score_string[100];
  FILE *file = fopen(HIGH_SCORE_PATH, "r");

  if (file) {
    while (fgets(high_score_string, 100, file)) {
      high_score = atoi(high_score_string);
    }
    fclose(file);
  }
  return high_score;
}

/**
 * @brief Generates a random figure
 *
 * This function returns a random number between 0 and 6, which
 * is used to select a figure type.
 *
 * @return A random number between 0 and 6
 */
int generate_figure() { return rand() % 7; }

/**
 * @brief Pauses or unpauses the game
 *
 * This function is used to toggle the pause state of the game. If the game is
 * not paused, it sets the pause flag to TRUE. If the game is paused, it sets
 * the flag to FALSE.
 *
 * @param game_info The game information structure, which contains the pause
 * flag
 */
void pause_game(Game_Info *game_info) {
  if (game_info->pause == STARTED) {
    game_info->pause = PAUSED;
  } else {
    game_info->pause = STARTED;
  }
}

void terminate_game(Game_Info *game_info) { game_info->pause = QUIT; }

void start_game(Tetromino *tet, Game_Info *game_info) {
  game_info->pause = STARTED;
  spawn_new_figure(tet, game_info, figures);
}

/**
 * @brief Copies a static 2D matrix to a dynamic 2D matrix
 *
 * This function copies the contents of a static 2D matrix to a dynamic 2D
 * matrix.
 *
 * @param dest The dynamic 2D matrix to copy to.
 * @param src The static 2D matrix to copy from.
 */
void stat_matrix_to_dyn(int **dest, int src[MAX_FIGURE_SIZE][MAX_FIGURE_SIZE]) {
  for (int i = 0; i < MAX_FIGURE_SIZE; i++) {
    for (int j = 0; j < MAX_FIGURE_SIZE; j++) {
      dest[i][j] = src[i][j];
    }
  }
}

/**
 * @brief Frees the memory allocated for a Tetromino structure.
 *
 * This function deallocates the memory used by the Tetromino structure,
 * including its figure array and the structure itself.
 *
 * @param tetromino Pointer to the Tetromino to be freed.
 */
void free_tetromino(Tetromino *tetromino) {
  if (tetromino == NULL) return;
  for (int i = 0; i < MAX_FIGURE_SIZE; i++) {
    free(tetromino->figure[i]);
  }
  free(tetromino->figure);
  free(tetromino);
}

/**
 * @brief Frees the memory allocated for a Game_Info structure.
 *
 * This function deallocates the memory used by the Game_Info structure,
 * including its field array, next array, and the structure itself.
 *
 * @param game_info Pointer to the Game_Info to be freed.
 */
void free_game(Game_Info *game_info) {
  if (game_info == NULL) return;

  for (int i = 0; i < MAX_FIGURE_SIZE; i++) {
    free(game_info->next[i]);
  }
  free(game_info->next);

  for (int i = 0; i < FIELD_H; i++) {
    free(game_info->field[i]);
  }
  free(game_info->field);
  free(game_info);
}
