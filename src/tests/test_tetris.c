#include <check.h>

#include "../inc/defines.h"
#include "../inc/tetris/fsm.h"

static int figures[FIGURES_COUNT][MAX_FIGURE_SIZE][MAX_FIGURE_SIZE] = {
    {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
    {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
    {{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
    {{0, 0, 0, 0}, {1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}},
    {{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
    {{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}};

START_TEST(test_1) {
  Game_Info *game_info = get_game_info();
  for (int y = 0; y < FIELD_H; y++) {
    for (int x = 0; x < FIELD_W; x++) {
      ck_assert_int_eq(game_info->field[y][x], 0);
    }
  }
  ck_assert_int_eq(game_info->level, 0);
  ck_assert_int_eq(game_info->speed, SPEED_1);
  ck_assert_int_eq(game_info->pause, 0);

  pause_game(game_info);
  ck_assert_int_eq(game_info->pause, 1);
  pause_game(game_info);

  for (int y = 16; y < FIELD_H; y++) {
    for (int x = 0; x < FIELD_W; x++) {
      game_info->field[y][x] = 1;
    }
  }

  game_info->field[16][FIELD_W - 1] = 0;

  clear_line(game_info);
  line_dropper(game_info);

  int result_field[FIELD_H][FIELD_W];

  for (int y = 0; y < FIELD_H; y++) {
    for (int x = 0; x < FIELD_W; x++) {
      result_field[y][x] = 0;
    }
  }

  for (int j = 0; j < FIELD_W - 1; j++) {
    result_field[FIELD_H - 1][j] = 1;
  }

  for (int y = 0; y < FIELD_H; y++) {
    for (int x = 0; x < FIELD_W; x++) {
      ck_assert_int_eq(result_field[y][x], game_info->field[y][x]);
    }
  }

  Tetromino *tetromino = set_tetromino(game_info);
  tetromino->is_placed = TRUE;
  place_tetromino_on_field(tetromino, game_info);
  for (int y = 0; y < MAX_FIGURE_SIZE; y++) {
    for (int x = 0; x < MAX_FIGURE_SIZE; x++) {
      if (tetromino->figure[y][x]) {
        int y_block = tetromino->coord.y + y;
        int x_block = tetromino->coord.x + x;
        result_field[y_block][x_block] = 1;
      }
    }
  }

  for (int y = 0; y < FIELD_H; y++) {
    for (int x = 0; x < FIELD_W; x++) {
      ck_assert_int_eq(game_info->field[y][x], result_field[y][x]);
    }
  }
  free_game(game_info);
  free_tetromino(tetromino);
}
END_TEST

START_TEST(test_2) {
  Game_Info *game_info = get_game_info();
  for (int y = 0; y < FIELD_H; y++) {
    for (int x = 0; x < FIELD_W; x++) {
      game_info->field[y][x] = 0;
    }
  }
  Tetromino *tetromino = set_tetromino(game_info);

  get_signal(tetromino, game_info, Start);
  ck_assert_int_eq(tetromino->can_spawn, 1);

  get_signal(tetromino, game_info, Pause);
  ck_assert_int_eq(game_info->pause, 2);

  pause_game(game_info);
  get_signal(tetromino, game_info, Left);
  ck_assert_int_eq(tetromino->coord.x, START_POS_FIGURE_X - 1);

  get_signal(tetromino, game_info, Right);
  ck_assert_int_eq(tetromino->coord.x, START_POS_FIGURE_X);

  get_signal(tetromino, game_info, Down);
  ck_assert_int_eq(tetromino->coord.y, START_POS_FIGURE_Y + 1);

  tetromino->coord.x = FIELD_W / 2;
  tetromino->coord.y = FIELD_H / 2;

  int result_figure[MAX_FIGURE_SIZE][MAX_FIGURE_SIZE];

  for (int y = 0; y < MAX_FIGURE_SIZE; y++) {
    for (int x = 0; x < MAX_FIGURE_SIZE; x++) {
      result_figure[y][x] = tetromino->figure[MAX_FIGURE_SIZE - x - 1][y];
    }
  }

  get_signal(tetromino, game_info, Action);

  for (int y = 0; y < MAX_FIGURE_SIZE; y++) {
    for (int x = 0; x < MAX_FIGURE_SIZE; x++) {
      ck_assert_int_eq(tetromino->figure[y][x], result_figure[y][x]);
    }
  }

  free_game(game_info);
  free_tetromino(tetromino);
}
END_TEST

START_TEST(test_3) {
  Game_Info *game_info = get_game_info();
  score_update(game_info, 1);
  ck_assert_int_eq(game_info->score, 100);
  score_update(game_info, 2);
  ck_assert_int_eq(game_info->score, 400);
  score_update(game_info, 3);
  ck_assert_int_eq(game_info->score, 1100);
  score_update(game_info, 4);
  ck_assert_int_eq(game_info->score, 2600);
  level_speed_update(game_info);
  ck_assert_int_eq(game_info->level, 4);
  free_game(game_info);
}
END_TEST

START_TEST(test_4) {
  Game_Info *game_info = get_game_info();
  game_info->high_score = 1000;
  save_high_score(game_info);

  int high_score = 0;
  FILE *file = fopen(HIGH_SCORE_PATH, "r");

  if (file) {
    char high_score_str[100];
    while (fgets(high_score_str, 100, file)) high_score = atoi(high_score_str);
    fclose(file);
  }
  ck_assert_int_eq(game_info->high_score, high_score);

  free_game(game_info);
}
END_TEST

START_TEST(test_5) {
  Game_Info *game_info = get_game_info();
  Tetromino *tetromino = set_tetromino(game_info);
  spawn_new_figure(tetromino, game_info, figures);
  ck_assert_int_eq(tetromino->can_spawn, 1);

  free_tetromino(tetromino);
  free_game(game_info);
}
END_TEST

Suite *test_backend_core() {
  Suite *s = suite_create("\033[33mstest_backend\033[0m");
  TCase *tc_core = tcase_create("backed_test");

  tcase_add_test(tc_core, test_1);
  tcase_add_test(tc_core, test_2);
  tcase_add_test(tc_core, test_3);
  tcase_add_test(tc_core, test_4);
  tcase_add_test(tc_core, test_5);

  suite_add_tcase(s, tc_core);
  return s;
}

int main() {
  int all = 0, success = 0, fail = 0;
  Suite *suite[] = {test_backend_core(), NULL};
  for (int i = 0; suite[i] != 0; i++) {
    SRunner *sr = srunner_create(suite[i]);
    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_NORMAL);
    all += srunner_ntests_run(sr);
    fail += srunner_ntests_failed(sr);
    srunner_free(sr);
  }
  success = all - fail;
  printf("ALL: %d\nSUCCESS: %d\nFAIL: %d\n", all, success, fail);
  return fail == 0 ? 0 : 1;
}
