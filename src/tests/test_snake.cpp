#include <gtest/gtest.h>

#include "../inc/snake/snake.h"
using namespace s21;

TEST(SnakeModel, Constuctor) {
  Snake snake;

  for (int i = 0; i < FIELD_H; ++i) {
    ASSERT_NE(snake.GetField()[i], nullptr);
    for (int j = 0; j < FIELD_W; ++j) {
      EXPECT_EQ(snake.GetField()[i][j], 0);
    }
  }

  ASSERT_NE(snake.GetApple(), nullptr);
  ASSERT_NE(snake.GetApple()[0], nullptr);

  EXPECT_EQ(snake.GetLevel(), LEVEL_MIN);
  EXPECT_EQ(snake.GetSpeed(), SPEED_1_SNAKE);
  EXPECT_EQ(snake.GetPauseState(), NOT_STARTED);
  EXPECT_EQ(snake.GetScore(), NOT_STARTED);

  EXPECT_EQ(snake.GetDirection(), Up);
}

TEST(SnakeModel, InitSnake) {
  Snake snake;

  std::deque<std::pair<int, int>> expectedCoordinates = {
      {FIELD_W / 2, FIELD_H / 2 - 1},
      {FIELD_W / 2, FIELD_H / 2},
      {FIELD_W / 2, FIELD_H / 2 + 1},
      {FIELD_W / 2, FIELD_H / 2 + 2}};

  ASSERT_EQ(snake.snake_coordinates_.size(), expectedCoordinates.size());

  for (size_t i = 0; i < expectedCoordinates.size(); ++i) {
    EXPECT_EQ(snake.snake_coordinates_[i].x, expectedCoordinates[i].first);
    EXPECT_EQ(snake.snake_coordinates_[i].y, expectedCoordinates[i].second);
  }
}

TEST(SnakeModel, GenerateApple) {
  Snake snake;

  int appleX = snake.GetApple()[0][0];
  int appleY = snake.GetApple()[0][1];

  EXPECT_LE(appleX, FIELD_W);
  EXPECT_LE(appleY, FIELD_H);

  EXPECT_FALSE(snake.CheckSnakeBody(appleX, appleY));
}

TEST(SnakeModel, SnakeChangeDirection) {
  Snake snake;

  snake.MoveLeft();
  ASSERT_EQ(snake.GetDirection(), Left);

  snake.MoveDown();
  ASSERT_EQ(snake.GetDirection(), Down);

  snake.MoveRight();
  ASSERT_EQ(snake.GetDirection(), Right);

  snake.MoveUp();
  ASSERT_EQ(snake.GetDirection(), Up);

  snake.MoveDown();
  ASSERT_EQ(snake.GetDirection(), Up);

  snake.MoveRight();
  snake.MoveLeft();
  ASSERT_EQ(snake.GetDirection(), Right);
}

TEST(SnakeModel, ChangeState) {
  Snake snake;

  snake.StartGame();
  EXPECT_EQ(snake.GetPauseState(), STARTED);

  snake.PauseGame();
  EXPECT_EQ(snake.GetPauseState(), PAUSED);

  snake.PauseGame();
  EXPECT_EQ(snake.GetPauseState(), STARTED);

  snake.TerminateGame();
  EXPECT_EQ(snake.GetPauseState(), QUIT);
}

TEST(SnakeModel, CheckAteApple) {
  Snake snake;

  snake.snake_coordinates_ = {{5, 5}, {5, 6}, {5, 7}, {5, 8}};

  snake.SetAppleValue(0, 0, 5);
  snake.SetAppleValue(0, 1, 5);

  EXPECT_TRUE(snake.CheckAteApple());
  EXPECT_EQ(snake.GetScore(), 1);

  snake.SetAppleValue(0, 0, 10);
  snake.SetAppleValue(0, 1, 10);

  EXPECT_FALSE(snake.CheckAteApple());
  EXPECT_EQ(snake.GetScore(), 1);
}

TEST(SnakeModel, CheckAteItself) {
  Snake snake;

  snake.snake_coordinates_ = {{5, 5}, {5, 6}, {5, 7}, {5, 8}};

  EXPECT_FALSE(snake.CheckAteItself());

  snake.snake_coordinates_ = {{5, 5}, {5, 6}, {5, 7}, {5, 5}};

  EXPECT_TRUE(snake.CheckAteItself());
}

TEST(SnakeModel, CheckEndGame) {
  Snake snake;

  snake.snake_coordinates_ = {{5, 5}, {5, 6}, {5, 7}, {5, 5}};

  snake.CheckEndGame();
  EXPECT_EQ(snake.GetPauseState(), LOSED);

  snake.snake_coordinates_ = {{5, 5}, {5, 6}, {5, 7}, {5, 8}};
  snake.SetPauseState(STARTED);
  snake.SetScore(200);
  snake.CheckEndGame();
  EXPECT_EQ(snake.GetPauseState(), WIN);

  snake.snake_coordinates_ = {{1, 5}};
  snake.SetDirection(Left);
  snake.CheckEndGame();
  EXPECT_EQ(snake.GetPauseState(), LOSED);

  snake.snake_coordinates_ = {{FIELD_W, 5}};
  snake.SetDirection(Right);
  snake.CheckEndGame();
  EXPECT_EQ(snake.GetPauseState(), LOSED);

  snake.snake_coordinates_ = {{5, 1}};
  snake.SetDirection(Up);
  snake.CheckEndGame();
  EXPECT_EQ(snake.GetPauseState(), LOSED);

  snake.snake_coordinates_ = {{5, FIELD_H}};
  snake.SetDirection(Down);
  snake.CheckEndGame();
  EXPECT_EQ(snake.GetPauseState(), LOSED);
}

TEST(SnakeModel, LevelUpdate) {
  Snake snake;
  EXPECT_EQ(snake.GetLevel(), 0);

  snake.SetScore(5);
  snake.UpdateLevelSpeed();
  EXPECT_EQ(snake.GetLevel(), 1);

  snake.SetScore(10);
  snake.UpdateLevelSpeed();
  EXPECT_EQ(snake.GetLevel(), 2);

  snake.SetScore(15);
  snake.UpdateLevelSpeed();
  EXPECT_EQ(snake.GetLevel(), 3);

  snake.SetScore(20);
  snake.UpdateLevelSpeed();
  EXPECT_EQ(snake.GetLevel(), 4);

  snake.SetScore(25);
  snake.UpdateLevelSpeed();
  EXPECT_EQ(snake.GetLevel(), 5);

  snake.SetScore(30);
  snake.UpdateLevelSpeed();
  EXPECT_EQ(snake.GetLevel(), 6);

  snake.SetScore(35);
  snake.UpdateLevelSpeed();
  EXPECT_EQ(snake.GetLevel(), 7);

  snake.SetScore(40);
  snake.UpdateLevelSpeed();
  EXPECT_EQ(snake.GetLevel(), 8);

  snake.SetScore(45);
  snake.UpdateLevelSpeed();
  EXPECT_EQ(snake.GetLevel(), 9);

  snake.SetScore(50);
  snake.UpdateLevelSpeed();
  EXPECT_EQ(snake.GetLevel(), 10);
  snake.SaveHighScore();
  EXPECT_EQ(snake.GetHighScore(), snake.GetScore());
}

TEST(SnakeModel, MoveSnakeUp) {
  Snake snake;
  snake.SetPauseState(STARTED);
  auto initialHead = snake.snake_coordinates_.front();
  snake.MoveSnake(Up);
  auto newHead = snake.snake_coordinates_.front();

  EXPECT_EQ(newHead.x, initialHead.x);
  EXPECT_EQ(newHead.y, initialHead.y - 1);
  EXPECT_EQ(snake.snake_coordinates_.size(), 4);
}

TEST(SnakeModel, MoveSnakeDown) {
  Snake snake;
  snake.SetPauseState(STARTED);
  auto initialHead = snake.snake_coordinates_.front();
  snake.MoveSnake(Down);
  auto newHead = snake.snake_coordinates_.front();

  EXPECT_EQ(newHead.x, initialHead.x);
  EXPECT_EQ(newHead.y, initialHead.y + 1);
  EXPECT_EQ(snake.snake_coordinates_.size(), 4);
}

TEST(SnakeModel, MoveSnakeLeft) {
  Snake snake;
  snake.SetPauseState(STARTED);
  auto initialHead = snake.snake_coordinates_.front();
  snake.MoveSnake(Left);
  auto newHead = snake.snake_coordinates_.front();

  EXPECT_EQ(newHead.x, initialHead.x - 1);
  EXPECT_EQ(newHead.y, initialHead.y);
  EXPECT_EQ(snake.snake_coordinates_.size(), 4);
}

TEST(SnakeModel, MoveSnakeRight) {
  Snake snake;
  snake.SetPauseState(STARTED);
  auto initialHead = snake.snake_coordinates_.front();
  snake.MoveSnake(Right);
  auto newHead = snake.snake_coordinates_.front();

  EXPECT_EQ(newHead.x, initialHead.x + 1);
  EXPECT_EQ(newHead.y, initialHead.y);
  EXPECT_EQ(snake.snake_coordinates_.size(), 4);
}

TEST(SnakeModel, Restart) {
  Snake snake;

  snake.snake_coordinates_ = {{5, 5}, {5, 6}, {5, 7}, {5, 8}};

  snake.SetAppleValue(0, 0, 5);
  snake.SetAppleValue(0, 1, 5);

  snake.SetScore(15);
  snake.UpdateLevelSpeed();

  snake.ResetSnake();

  ASSERT_NE(snake.GetApple(), nullptr);
  ASSERT_NE(snake.GetApple()[0], nullptr);

  EXPECT_EQ(snake.GetLevel(), LEVEL_MIN);
  EXPECT_EQ(snake.GetSpeed(), SPEED_1_SNAKE);
  EXPECT_EQ(snake.GetPauseState(), NOT_STARTED);
  EXPECT_EQ(snake.GetScore(), NOT_STARTED);

  EXPECT_EQ(snake.GetDirection(), Up);
}