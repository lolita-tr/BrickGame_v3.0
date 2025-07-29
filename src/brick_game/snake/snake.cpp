#include "../../inc/snake/snake.h"
#include "../../inc/game_common.h"

/** @file */

namespace s21 {
/**
 * @brief Construct a new Snake object
 *
 * Constructor for Snake class. Seed for random is set.
 * Memory for game field and apple is allocated.
 * High score is set from file, level speed and pause status are set to default.
 * Direction is set to Up and last time is set to current time.
 * Snake is initialized and apple is generated.
 */
Snake::Snake() {
  std::srand(std::time(nullptr));

  game_info_.field = new int *[FIELD_H];
  for (int i = 0; i < FIELD_H; i++) {
    game_info_.field[i] = new int[FIELD_H]();
  }

  game_info_.next = new int *[1];
  game_info_.next[0] = new int[2]();
  game_info_.next[0][0] = -1;  // Инициализируем с невалидными координатами
  game_info_.next[0][1] = -1;

  game_info_.high_score = get_high_score_from_file(HIGH_SCORE_PATH_SNAKE);
  game_info_.level = LEVEL_MIN;
  game_info_.speed = SPEED_1_SNAKE;
  game_info_.pause = NOT_STARTED;
  game_info_.score = NOT_STARTED;

  direction_ = Up;
  last_time_ = clock();
  move_flag_ = true;
  InitSnake();
  GenerateApple();
}

/**
 * @brief Destructor for Snake class
 *
 * Deallocates memory for game field and apple.
 */
Snake::~Snake() {
  for (int i = 0; i < FIELD_H; ++i) {
    delete[] game_info_.field[i];
  }
  delete[] game_info_.field;

  delete[] game_info_.next[0];
  delete[] game_info_.next;
}

/**
 * @brief Generates a new apple at random position on the field
 *
 * This method ensures that the generated apple does not overlap with the
 * snake's body.
 */
void Snake::GenerateApple() {
  
  SnakeElements position;

  do {
    position.x = rand() % FIELD_W;
    position.y = rand() % FIELD_H;
  } while (CheckSnakeBody(position.x, position.y));

  game_info_.next[0][0] = position.x;
  game_info_.next[0][1] = position.y;

  game_info_.field[position.y][position.x] = 3;  // 3 - яблоко
}

/**
 * @brief Initializes the snake's starting position on the field.
 *
 * This method sets the initial coordinates for the snake's body segments
 * at the center of the field. The snake is initialized in a vertical
 * orientation with a default length.
 */

void Snake::InitSnake() {
  snake_coordinates_ = {{FIELD_W / 2, FIELD_H / 2 - 1},
                        {FIELD_W / 2, FIELD_H / 2},
                        {FIELD_W / 2, FIELD_H / 2 + 1},
                        {FIELD_W / 2, FIELD_H / 2 + 2}};
  
  // Сохраняем координаты змейки в игровое поле
  // 1 - тело змейки, 2 - голова змейки
  for (size_t i = 0; i < snake_coordinates_.size(); ++i) {
    int x = snake_coordinates_[i].x;
    int y = snake_coordinates_[i].y;
    if (i == 0) {
      // Голова змейки
      game_info_.field[y][x] = 2;
    } else {
      // Тело змейки
      game_info_.field[y][x] = 1;
    }
  }
}

/**
 * @brief Moves the snake in the specified direction.
 *
 * This method updates the position of the snake's head based
 * on the user action and checks for game conditions such as
 * eating an apple or ending the game. If the game is paused,
 * the snake does not move. If the snake eats an apple, a new
 * apple is generated and the game's speed is updated.
 *
 * @param action The direction in which to move the snake.
 */

void Snake::MoveSnake(UserAction action) {
  CheckEndGame();
  if (game_info_.pause != STARTED) {
    return;
  }
  SnakeElements head = snake_coordinates_.front();

  switch (action) {
    case Up:
      head.y -= 1;
      break;
    case Down:
      head.y += 1;
      break;
    case Left:
      head.x -= 1;
      break;
    case Right:
      head.x += 1;
      break;
    default:
      return;
  }

  // Проверяем, съела ли змейка яблоко
  bool ate_apple = CheckAteApple();
  snake_coordinates_.push_front(head);


  if (ate_apple) {
    // Змейка съела яблоко - увеличиваем счет и генерируем новое яблоко
    GenerateApple();
    UpdateLevelSpeed();
  } else {
    // Змейка не съела яблоко - удаляем хвост и очищаем его из поля
    SnakeElements tail = snake_coordinates_.back();
    game_info_.field[tail.y][tail.x] = 0;
    snake_coordinates_.pop_back();
  }

  // Обновляем поле с новыми позициями змейки
  for (size_t i = 0; i < snake_coordinates_.size(); ++i) {
    int x = snake_coordinates_[i].x;
    int y = snake_coordinates_[i].y;
    if (i == 0) {
      // Голова змейки
      game_info_.field[y][x] = 2;
    } else {
      // Тело змейки
      game_info_.field[y][x] = 1;
    }
  }

  move_flag_ = true;
}

/**
 * @brief Changes the snake's direction to left.
 *
 * This method sets the snake's direction to left
 * unless the current direction is right, to prevent
 * the snake from reversing directly onto itself.
 */

void Snake::MoveLeft() {
  if (direction_ != Right) {
    direction_ = Left;
    move_flag_ = false;
  }
}

/**
 * @brief Changes the snake's direction to right.
 *
 * This method sets the snake's direction to right unless
 * the current direction is left, to prevent the snake from
 * reversing directly onto itself.
 */
void Snake::MoveRight() {
  if (direction_ != Left) {
    direction_ = Right;
    move_flag_ = false;
  }
}

/**
 * @brief Changes the snake's direction to down.
 *
 * This method sets the snake's direction to down unless
 * the current direction is up, to prevent the snake from
 * reversing directly onto itself.
 */

void Snake::MoveDown() {
  if (direction_ != Up) {
    direction_ = Down;
    move_flag_ = false;
  }
}

/**
 * @brief Changes the snake's direction to up.
 *
 * This method sets the snake's direction to up
 * unless the current direction is down, to prevent
 * the snake from reversing directly onto itself.
 */

void Snake::MoveUp() {
  if (direction_ != Down) {
    direction_ = Up;
    move_flag_ = false;
  }
}

/**
 * @brief Ends the game.
 *
 * This method ends the game by setting the pause field of GameInfo to QUIT.
 */
void Snake::TerminateGame() { game_info_.pause = QUIT; }

/**
 * @brief Starts the game.
 *
 * This method sets the pause field of GameInfo to STARTED,
 * which starts the game.
 */
void Snake::StartGame() { game_info_.pause = STARTED; }

/**
 * @brief Toggles the pause state of the game.
 *
 * If the game is running, this method sets the pause field of GameInfo to
 * PAUSED. If the game is paused, this method sets the pause field of GameInfo
 * to STARTED.
 */
void Snake::PauseGame() {
  game_info_.pause = (game_info_.pause == PAUSED) ? STARTED : PAUSED;
}

/**
 * @brief Checks if the snake has eaten the apple.
 *
 * This function compares the coordinates of the snake's head
 * with the coordinates of the apple. If they match, it increments
 * the score and returns true, indicating the snake has eaten the apple.
 * Otherwise, it returns false.
 *
 * @return true if the snake has eaten the apple, false otherwise.
 */

bool Snake::CheckAteApple() {
  if (snake_coordinates_.front().x == game_info_.next[0][0] &&
      snake_coordinates_.front().y == game_info_.next[0][1]) {
    ++game_info_.score;
    return true;
  }
  return false;
}

/**
 * @brief Checks if the snake has eaten itself.
 *
 * This function checks if the coordinates of the snake's head
 * match with any of the coordinates of the snake's body.
 * If they match, it returns true, indicating the snake has eaten itself.
 * Otherwise, it returns false.
 *
 * @return true if the snake has eaten itself, false otherwise.
 */
bool Snake::CheckAteItself() {
  for (size_t i = 1; i < snake_coordinates_.size(); ++i) {
    if (snake_coordinates_[0].x == snake_coordinates_[i].x &&
        snake_coordinates_[0].y == snake_coordinates_[i].y) {
      return true;
    }
  }
  return false;
}

/**
 * @brief Checks if the given coordinates are part of the snake's body.
 *
 * This function iterates over the snake's coordinates and checks if the given
 * coordinates match with any of them. If they match, it returns true,
 * indicating the coordinates are part of the snake's body. Otherwise, it
 * returns false.
 *
 * @param[in] x the x-coordinate to check
 * @param[in] y the y-coordinate to check
 *
 * @return true if the coordinates are part of the snake's body, false
 * otherwise.
 */
bool Snake::CheckSnakeBody(int x, int y) {
  // Проверяем границы поля
  if (x < 0 || x >= FIELD_W || y < 0 || y >= FIELD_H) {
    return true; // За пределами поля считаем занятым
  }
  
  // Проверяем поле напрямую - если там есть змейка (1 или 2) или яблоко (3)
  return (game_info_.field[y][x] == 1 || game_info_.field[y][x] == 2 || game_info_.field[y][x] == 3);
}

/**
 * @brief Checks if the game is over.
 *
 * This function checks if the game is over by verifying
 * if the snake has hit the edge of the field or has eaten
 * itself. If the game is over, it sets the pause field of
 * GameInfo to LOSED or WIN. If the snake has hit the edge
 * of the field, it sets the pause field to LOSED. If the
 * snake has eaten itself, it also sets the pause field
 * to LOSED. If the score is 200, it sets the pause field
 * to WIN.
 */
void Snake::CheckEndGame() {
  for (size_t i = 0; i < snake_coordinates_.size(); ++i) {
    if (snake_coordinates_.front().x <= 0 && GetDirection() == Left) {
      game_info_.pause = LOSED;
    } else if (snake_coordinates_.front().x >= FIELD_W - 1 &&
               GetDirection() == Right) {
      game_info_.pause = LOSED;
    } else if (snake_coordinates_.front().y <= 0 && GetDirection() == Up) {
      game_info_.pause = LOSED;
    } else if (snake_coordinates_.front().y >= FIELD_H - 1 &&
               GetDirection() == Down) {
      game_info_.pause = LOSED;
    } else if (game_info_.score == 200) {
      game_info_.pause = WIN;
    }
  }

  if (CheckAteItself()) {
    game_info_.pause = LOSED;
  }
}

UserAction Snake::GetDirection() { return this->direction_; }

/**
 * @brief Gets the current high score from a file.
 *
 * This function reads the high score from a file specified by
 * HIGH_SCORE_PATH_SNAKE. It reads the file line by line and converts each line
 * to an integer, storing the last value read in the high_score variable. The
 * file is then closed and the high score is returned.
 *
 * @return The high score read from the file
 */
int Snake::GetHighScore() {
  return get_high_score_from_file(HIGH_SCORE_PATH_SNAKE);
}

/**
 * @brief Saves the current score as the high score if it exceeds the existing
 * one.
 *
 * This function compares the current game score with the high score.
 * If the current score is greater than or equal to the high score, it
 * updates the high score by writing it to a file specified by
 * HIGH_SCORE_PATH_SNAKE. The file is opened in write mode, and
 * if successful, the high score is written to it, and the file is closed.
 */

void Snake::SaveHighScore() {
  if (game_info_.score >= game_info_.high_score) {
    save_high_score_to_file(HIGH_SCORE_PATH_SNAKE, game_info_.score);
  }
}

/**
 * @brief Updates the level and speed of the game based on the score.
 *
 * This function checks if the game level is less than LEVEL_MAX.
 * If it is, it checks if the score is divisible by 5.
 * If the score is divisible by 5, it increments the level and
 * sets the speed to SPEED_1_SNAKE minus SPEED_STEP_SNAKE.
 * It then checks if the current score is greater than the high score.
 * If it is, it updates the high score and calls SaveHighScore.
 */
void Snake::UpdateLevelSpeed() {
  update_level_speed(&game_info_, SPEED_STEP_SNAKE);

  if (game_info_.score > game_info_.high_score) {
    game_info_.high_score = game_info_.score;
    SaveHighScore();
  }
}

/**
 * @brief Resets the game to its initial state.
 *
 * This function resets the game to its initial state by deleting
 * the existing game field and apple, and allocating new memory for
 * them. It also resets the game's state variables such as the high
 * score, level, speed, pause, and score. It then calls the
 * GenerateApple and InitSnake functions to reset the game's state.
 *
 */
void Snake::ResetSnake() {
  for (int i = 0; i < FIELD_H; ++i) {
    delete[] game_info_.field[i];
  }
  delete[] game_info_.field;

  game_info_.field = new int *[FIELD_H];
  for (int i = 0; i < FIELD_H; ++i) {
    game_info_.field[i] = new int[FIELD_H]();
  }

  for (int i = 0; i < 1; ++i) {
    delete[] game_info_.next[i];
  }
  delete[] game_info_.next;

  game_info_.next = new int *[1];
  game_info_.next[0] = new int[2]();
  game_info_.next[0][0] = -1;
  game_info_.next[0][1] = -1;


  game_info_.high_score = get_high_score_from_file(HIGH_SCORE_PATH_SNAKE);
  game_info_.level = LEVEL_MIN;
  game_info_.speed = SPEED_1_SNAKE;
  game_info_.pause = NOT_STARTED;
  game_info_.score = NOT_STARTED;

  direction_ = Up;
  last_time_ = clock();

  GenerateApple();
  InitSnake();
}

}  // namespace s21