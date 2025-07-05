/** @file */

#ifndef CPP3_S21_BrickGame2_SRC_INC_DEFINES_H
#define CPP3_S21_BrickGame2_SRC_INC_DEFINES_H

#define FIELD_H 20
#define FIELD_W 10

#define NOT_STARTED 0
#define STARTED 1
#define PAUSED 2
#define LOSED 3
#define QUIT 4
#define WIN 5

#define FIELD_PADDING 1

#define SHIFT_X 20
#define SHIFT_Y 10

#define CELL_SIZE 20

#define FIELD_HEIGHT (FIELD_H + FIELD_PADDING)
#define FIELD_WIDTH (FIELD_W + FIELD_PADDING)

#define HIGH_SCORE_PATH "build/high_score.txt"
#define HIGH_SCORE_PATH_SNAKE "build/high_score_snake.txt"

#define START_POS_FIGURE_X 4
#define START_POS_FIGURE_Y 0

#define MAX_FIGURE_SIZE 4
#define FIGURES_COUNT 7

#define LEVEL_MIN 0
#define LEVEL_MAX 10
#define SPEED_1 100000
#define SPEED_1_SNAKE 500
#define SPEED_STEP_SNAKE 50

#endif  // CPP3_S21_BrickGame2_SRC_INC_DEFINES_H
