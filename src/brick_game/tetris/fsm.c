#include "../../inc/tetris/fsm.h"

/** @file */

/**
 * @brief Function to handle all the possible actions
 *
 * @param tet The struct which contains all the information about the current
 * figure
 * @param game_info The struct which contains all the information about the game
 * @param action The action to be taken
 *
 * @return void
 */
void get_signal(Tetromino *tet, Game_Info *game_info, Signals action) {
  switch (action) {
    case Start:
      start_game(tet, game_info);
      break;
    case Pause:
      pause_game(game_info);
      break;
    case Action:
      rotate_tetromino(tet, game_info);
      break;
    case Down:
      drop_tetromino(tet, game_info);
      break;
    case Right:
      move_tetromino_right(tet, game_info);
      break;
    case Left:
      move_tetromino_left(tet, game_info);
      break;
    case Terminate:
      terminate_game(game_info);
      break;
    default:
      break;
  }
}
