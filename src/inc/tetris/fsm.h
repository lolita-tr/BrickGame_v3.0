/** @file */

#ifndef CPP3_S21_BrickGame2_SRC_INC_TETRIS_FSM_H_
#define CPP3_S21_BrickGame2_SRC_INC_TETRIS_FSM_H_

#include "../defines.h"
#include "fsm.h"
#include "tetris_frontend.h"

#ifdef __cplusplus
extern "C" {
#endif

void get_signal(Tetromino *tet, Game_Info *game_info, Signals action);

#ifdef __cplusplus
}
#endif

#endif  // CPP3_S21_BrickGame2_SRC_INC_TETRIS_FSM_H_