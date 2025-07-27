/** @file */

#ifndef CPP3_S21_BrickGame2_SRC_INC_SNAKE_SNAKE_H_
#define CPP3_S21_BrickGame2_SRC_INC_SNAKE_SNAKE_H_

#include <ncurses.h>

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <iostream>

#include "../defines.h"
#include "../../inc/game_common.h"

namespace s21 {
// Using common GameInfo and UserAction from game_common.h

class Snake {
 public:
  struct SnakeElements {
    int x;
    int y;
  };

  Snake();
  ~Snake();

  void StartGame();
  void GenerateApple();
  void InitSnake();
  void MoveSnake(UserAction action);
  void CheckEndGame();

  void MoveLeft();
  void MoveRight();
  void MoveUp();
  void MoveDown();

  void TerminateGame();
  void PauseGame();

  bool CheckAteApple();
  bool CheckAteItself();
  bool CheckSnakeBody(int x, int y);

  UserAction GetDirection();
  void SetDirection(UserAction direction) { direction_ = direction; };

  int GetHighScore();
  void SaveHighScore();
  void UpdateLevelSpeed();
  void ResetSnake();

  // Access to apple coordinates (stored in next field of GameInfo)

  const GameInfo& GetGameInfo() const { return game_info_; };
  void SetGameInfo(const GameInfo& game_info) { game_info_ = game_info; };

  const int* const* GetField() const { return game_info_.field; };
  const int* const* GetApple() const { return game_info_.next; };

  void SetField(int** new_field) { game_info_.field = new_field; }
  void SetApple(int** new_apple) { game_info_.next = new_apple; }

  void SetAppleValue(int row, int col, int value) {
    if (game_info_.next != nullptr) {
      game_info_.next[row][col] = value;
    }
  }

  int GetScore() const { return game_info_.score; };
  void SetScore(int score) { game_info_.score = score; };

  int GetHighScore() const { return game_info_.high_score; };
  void SetHighScore(int high_score) { game_info_.high_score = high_score; };

  int GetLevel() const { return game_info_.level; };
  void SetLevel(int level) { game_info_.level = level; };

  int GetSpeed() const { return game_info_.speed; };
  void SetSpeed(int speed) { game_info_.speed = speed; };

  int GetPauseState() const { return game_info_.pause; };
  void SetPauseState(int pause) { game_info_.pause = pause; };

  int GetMoveFlag() const { return move_flag_; };

  clock_t last_time_;
  std::deque<SnakeElements> snake_coordinates_;

 private:
  UserAction direction_;
  GameInfo game_info_;
  bool move_flag_;
};
}  // namespace s21

#endif  // CPP3_S21_BrickGame2_SRC_INC_SNAKE_SNAKE_H_