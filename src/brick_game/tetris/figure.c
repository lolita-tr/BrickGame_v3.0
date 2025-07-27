#include "../../inc/tetris/figures.h"
#include "../../inc/tetris/tetris.h"
#include "../../inc/game_common.h"
/** @file */

/**
 * @brief Set tetromino structure. If tetromino is NULL, allocates memory for
 * it, sets start position, sets type and next type of tetromino, allocates
 * memory for figure and sets it.
 *
 * @param game_info - pointer to Game_Info structure
 *
 * @return Pointer to Tetromino structure
 */
Tetromino *set_tetromino(GameInfo *game_info) {
  Tetromino *tetromino = calloc(1, sizeof(Tetromino));

  tetromino->type = generate_figure();
  tetromino->next_type = generate_figure();

  stat_matrix_to_dyn(game_info->next, figures[tetromino->next_type]);

  tetromino->figure = calloc(MAX_FIGURE_SIZE, sizeof(int *));

  for (int row = 0; row < MAX_FIGURE_SIZE; row++) {
    tetromino->figure[row] = calloc(MAX_FIGURE_SIZE, sizeof(int));
  }

  stat_matrix_to_dyn(tetromino->figure, figures[tetromino->type]);
  set_start_position_for_tetromino(tetromino);

  tetromino->is_placed = FALSE;
  tetromino->can_spawn = TRUE;

  return tetromino;
}

/**
 * @brief Sets the starting position for a tetromino.
 *
 * This function initializes the coordinates of the tetromino
 * to predefined starting positions.
 *
 * @param tetromino - pointer to the Tetromino structure to be positioned
 */
void set_start_position_for_tetromino(Tetromino *tetromino) {
  tetromino->coord.y = START_POS_FIGURE_Y;
  tetromino->coord.x = START_POS_FIGURE_X;
}

/**
 * @brief Generates a new tetromino and moves the previously generated one to
 * the playing field.
 *
 * This function takes the previously generated tetromino and moves it to the
 * playing field. It then generates a new tetromino and assigns it to the
 * next_type field of the tetromino structure. The newly generated tetromino is
 * then set to the starting position and its is_spawned flag is reset.
 *
 * @param tetromino - pointer to the Tetromino structure
 * @param game_info - pointer to the Game_Info structure
 * @param figures - array of figures
 */
void generate_next_tetromino(Tetromino *tetromino, GameInfo *game_info,
                             int figures[7][4][4]) {
  tetromino->type = tetromino->next_type;
  for (int y = 0; y < MAX_FIGURE_SIZE; y++) {
    for (int x = 0; x < MAX_FIGURE_SIZE; x++) {
      tetromino->figure[y][x] = game_info->next[y][x];
    }
  }

  tetromino->next_type = generate_figure();
  stat_matrix_to_dyn(game_info->next, figures[tetromino->next_type]);

  set_start_position_for_tetromino(tetromino);

  tetromino->is_placed = FALSE;
}

/**
 * @brief Spawns a new tetromino on the game field
 *
 * This function takes a tetromino and the game information structure as
 * parameters. It checks if the tetromino is placed and if it is, it places the
 * tetromino on the game field by calling the place_tetromino_on_field function.
 * It then generates a new tetromino by calling the generate_next_tetromino
 * function and checks if the game is over by calling the check_game_over
 * function. The can_spawn flag is set to FALSE until the newly spawned
 * tetromino is placed again.
 *
 * @param tetromino - pointer to the Tetromino structure
 * @param game_info - pointer to the Game_Info structure
 * @param figures - array of figures
 */
void spawn_new_figure(Tetromino *tetromino, GameInfo *game_info,
                      int figures[7][4][4]) {
  if (tetromino->is_placed && tetromino->can_spawn) {
    place_tetromino_on_field(tetromino, game_info);
    generate_next_tetromino(tetromino, game_info, figures);
    check_game_over(tetromino, game_info);
  }
}

/**
 * @brief Moves a tetromino one row down.
 *
 * This function takes a tetromino and the game information structure as
 * parameters. It checks if the tetromino can be moved one row down by checking
 * if the block below is empty and if the game is not paused. If the tetromino
 * can be moved, it moves it and if not, it sets the is_placed flag to TRUE.
 *
 * @param tet - pointer to the Tetromino structure
 * @param game_info - pointer to the Game_Info structure
 */
void move_tetromino_down_one_row(Tetromino *tet, GameInfo *game_info) {
  int can_move = 1;
  for (int y = 0; y < MAX_FIGURE_SIZE; y++) {
    for (int x = 0; x < MAX_FIGURE_SIZE; x++) {
      if (tet->figure[y][x]) {
        int y_block = tet->coord.y + y;
        int x_block = tet->coord.x + x;
        if (y_block + 1 > FIELD_H - 1 ||
            game_info->field[y_block + 1][x_block] ||
            game_info->pause == PAUSED) {
          tet->is_placed = TRUE;
          can_move = 0;
          break;
        }
      }
    }
    if (!can_move) {
      break;
    }
  }

  if (can_move) {
    tet->coord.y++;
  }
}

/**
 * @brief Drops the tetromino quickly to the lowest possible position.
 *
 * This function moves the tetromino down one row if the game is not paused.
 * It leverages the move_tetromino_down_one_row function to perform the
 * movement. The tetromino will continue to drop until it is placed,
 * allowing the player to quickly set the piece at the bottom of the field.
 *
 * @param tet - pointer to the Tetromino structure
 * @param game_info - pointer to the Game_Info structure
 */
void drop_tetromino(Tetromino *tet, GameInfo *game_info) {
  if (game_info->pause == STARTED) move_tetromino_down_one_row(tet, game_info);
}

/**
 * @brief Moves the tetromino one column to the left if the game is not paused.
 *
 * This function moves the tetromino one column to the left if the game is not
 * paused. The tetromino will not be moved if it is already at the leftmost
 * column of the field, or if there is already a block in the column to the
 * left of the tetromino.
 *
 * @param tet - pointer to the Tetromino structure
 * @param game_info - pointer to the Game_Info structure
 */
void move_tetromino_left(Tetromino *tet, GameInfo *game_info) {
  int can_move = 1;
  for (int y = 0; y < MAX_FIGURE_SIZE; y++) {
    for (int x = 0; x < MAX_FIGURE_SIZE; x++) {
      if (tet->figure[y][x]) {
        int y_block = tet->coord.y + y;
        int x_block = tet->coord.x + x;
        if (x_block - 1 < 0 || game_info->field[y_block][x_block - 1] ||
            game_info->pause == PAUSED) {
          can_move = 0;
          break;
        }
      }
    }
    if (!can_move) {
      break;
    }
  }

  if (can_move) {
    tet->coord.x--;
  }
}

/**
 * @brief Moves the tetromino one column to the right if the game is not paused.
 *
 * This function moves the tetromino one column to the right if the game is not
 * paused. The tetromino will not be moved if it is already at the rightmost
 * column of the field, or if there is already a block in the column to the
 * right of the tetromino.
 *
 * @param tet - pointer to the Tetromino structure
 * @param game_info - pointer to the Game_Info structure
 */
void move_tetromino_right(Tetromino *tet, GameInfo *game_info) {
  int can_move = 1;
  for (int y = 0; y < MAX_FIGURE_SIZE; y++) {
    for (int x = 0; x < MAX_FIGURE_SIZE; x++) {
      if (tet->figure[y][x]) {
        int y_block = tet->coord.y + y;
        int x_block = tet->coord.x + x;
        if (x_block + 1 > FIELD_W - 1 ||
            game_info->field[y_block][x_block + 1] ||
            game_info->pause == PAUSED) {
          can_move = 0;
          break;
        }
      }
    }
    if (!can_move) {
      break;
    }
  }

  if (can_move) {
    tet->coord.x++;
  }
}

/**
 * @brief Rotates the tetromino clockwise if the game is not paused.
 *
 * This function rotates the tetromino clockwise if the game is not paused.
 * The tetromino will not be rotated if it is already at the edge of the field,
 * or if there is already a block in a position that the tetromino would occupy
 * after the rotation.
 *
 * @param tet - pointer to the Tetromino structure
 * @param game_info - pointer to the Game_Info structure
 */
void rotate_tetromino(Tetromino *tet, GameInfo *game_info) {
  int rotated[MAX_FIGURE_SIZE][MAX_FIGURE_SIZE];

  int can_rotate = 1;
  for (int y = 0; y < MAX_FIGURE_SIZE; y++) {
    for (int x = 0; x < MAX_FIGURE_SIZE; x++) {
      rotated[y][x] = tet->figure[4 - x - 1][y];
    }
  }

  for (int y = 0; y < MAX_FIGURE_SIZE && can_rotate; y++) {
    for (int x = 0; x < MAX_FIGURE_SIZE && can_rotate; x++) {
      if (rotated[y][x]) {
        int y_block = tet->coord.y + y;
        int x_block = tet->coord.x + x;
        if (y_block + 1 > FIELD_H - 1 || x_block > FIELD_W - 1 || x_block < 0 ||
            game_info->field[y_block][x_block] || game_info->pause == PAUSED) {
          can_rotate = 0;
        }
      }
    }
  }

  if (can_rotate) {
    for (int i = 0; i < MAX_FIGURE_SIZE; i++) {
      for (int j = 0; j < MAX_FIGURE_SIZE; j++) {
        tet->figure[i][j] = rotated[i][j];
      }
    }
  }
}
