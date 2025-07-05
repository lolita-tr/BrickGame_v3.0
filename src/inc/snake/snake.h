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

namespace s21 {
struct GameInfo {
  int** field;
  int** apple;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
};

typedef enum {
  Start,
  Pause,
  Terminate,
  Action,
  Left,
  Right,
  Up,
  Down
} UserAction;

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

  const GameInfo& GetGameInfo() const { return game_info_; };
  void SetGameInfo(const GameInfo& game_info) { game_info_ = game_info; };

  const int* const* GetField() const { return game_info_.field; };
  const int* const* GetApple() const { return game_info_.apple; };

  void SetField(int** new_field) { game_info_.field = new_field; }
  void SetApple(int** new_apple) { game_info_.apple = new_apple; }

  void SetAppleValue(int row, int col, int value) {
    if (game_info_.apple != nullptr) {
      game_info_.apple[row][col] = value;
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