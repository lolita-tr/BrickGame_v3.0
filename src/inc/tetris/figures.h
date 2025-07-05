/** @file */

#ifndef CPP3_S21_BrickGame2_SRC_INC_TETRIS_FIGURES_H_
#define CPP3_S21_BrickGame2_SRC_INC_TETRIS_FIGURES_H_

#include "../defines.h"

static int figures[FIGURES_COUNT][MAX_FIGURE_SIZE][MAX_FIGURE_SIZE] = {
    {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
    {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
    {{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
    {{0, 0, 0, 0}, {1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}},
    {{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
    {{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}};

#endif  // CPP3_S21_BrickGame2_SRC_INC_TETRIS_FIGURES_H_