/** @file */

#ifndef CPP3_S21_BrickGame2_SRC_INC_SNAKE_CONTROLLER_H_
#define CPP3_S21_BrickGame2_SRC_INC_SNAKE_CONTROLLER_H_

#include "snake.h"

namespace s21 {
class SnakeController {
 public:
  explicit SnakeController(Snake &snake);
  ~SnakeController();

  void UserInput(UserAction action, bool hold);
  GameInfo UpdateCurrentState();
  void ResetController();

  Snake &snake_;
};
}  // namespace s21

#endif  // CPP3_S21_BrickGame2_SRC_INC_SNAKE_CONTROLLER_H_