/** @file */

#ifndef CPP3_S21_BrickGame2_SRC_INC_SNAKE_VIEW_H_
#define CPP3_S21_BrickGame2_SRC_INC_SNAKE_VIEW_H_

#include "snake_controller.h"
#include "../game_common.h"

namespace s21 {

class SnakeView {
 public:
  explicit SnakeView(SnakeController &controller);

  void HandelInput();
  void StartSnakeGame();

  void RefreshGame(WINDOW *win, const GameInfo &game_info/*, Snake &snake*/);
  void PrintField(WINDOW *win);
  void PrintInfoBar(WINDOW *win);
  void DrawGameField(WINDOW *win, const GameInfo &game_info);
  void PrintOtherMessage(WINDOW *win);

  SnakeController &controller_;
};

// void PrintRectangle(WINDOW *win, int top_y, int bottom_y, int left_x,
//                     int right_x);
void DrawMenuScreen(int choosen_point);
void HandleInputMenu(int *choosen_point);
void StartChoosenGame(int *choosen_option);
void InitNcurses();

}  // namespace s21

#endif  // CPP3_S21_BrickGame2_SRC_INC_SNAKE_VIEW_H_