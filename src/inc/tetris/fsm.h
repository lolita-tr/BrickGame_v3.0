/** @file */

#ifndef CPP3_S21_BrickGame2_SRC_INC_TETRIS_FSM_H_
#define CPP3_S21_BrickGame2_SRC_INC_TETRIS_FSM_H_

#include "../defines.h"
#include "../game_common.h"
#include "tetris.h"

#ifdef __cplusplus
extern "C" {
#endif

void get_signal(Tetromino *tet, GameInfo *game_info, UserAction action);

#ifdef __cplusplus
}
#endif

#endif  // CPP3_S21_BrickGame2_SRC_INC_TETRIS_FSM_H_