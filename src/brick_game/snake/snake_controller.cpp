#include "../../inc/snake/snake_controller.h"

/** @file */

namespace s21 {

SnakeController::SnakeController(Snake &snakeInstance)
    : snake_(snakeInstance) {}

/**
 * @brief Processes the user input action and updates the snake's state
 * accordingly.
 *
 * This function takes a user action and a hold flag as input and performs the
 * corresponding action on the snake. If the game is started, it handles
 * movement in the specified direction or an action command. Additionally, it
 * processes termination, start, and pause actions regardless of the game's
 * state.
 *
 * @param[in] action The user action to be processed (e.g., Up, Down, Left,
 * Right, Action, Terminate, Start, Pause).
 * @param[in] hold A boolean flag indicating whether the action button is being
 * held.
 */

void SnakeController::UserInput(s21::UserAction action, bool hold) {
  if (snake_.GetPauseState() == STARTED) {
    if (snake_.GetMoveFlag()) {
      switch (action) {
        case Up:
          snake_.MoveUp();
          break;

        case Down:
          snake_.MoveDown();
          break;

        case Right:
          snake_.MoveRight();
          break;

        case Left:
          snake_.MoveLeft();
          break;

        case Action:
          if (hold) snake_.MoveSnake(snake_.GetDirection());
          break;

        default:
          break;
      }
    }
  }
  switch (action) {
    case Terminate:
      snake_.TerminateGame();
      break;

    case Start:
      snake_.StartGame();
      break;

    case Pause:
      snake_.PauseGame();
      break;

    default:
      break;
  }
}

/**
 * @brief Updates the snake's state by moving it one step in the specified
 * direction and returns a GameInfo object with the current game state.
 *
 * This function is used to update the snake's state at a given frequency
 * (determined by the Snake's speed). It returns a GameInfo object with the
 * current state of the game, including the snake's position, score, and other
 * relevant data.
 *
 * @return A GameInfo object containing the current game state.
 */
GameInfo SnakeController::UpdateCurrentState() {
  GameInfo game;
  clock_t current_time = clock();
  if ((current_time - snake_.last_time_) >=
      snake_.GetSpeed() * CLOCKS_PER_SEC / 1000) {
    snake_.MoveSnake(snake_.GetDirection());
    snake_.last_time_ = current_time;
  }
  return game;
}

/**
 * @brief Resets the snake's state to the initial state.
 *
 * This function calls the Snake's ResetSnake function to reset the snake's
 * state to the initial state. This involves resetting the snake's position,
 * score, high score, level, speed, and pause state. It also resets the game's
 * state variables such as the game's field, apple, and snake elements.
 */
void SnakeController::ResetController() { snake_.ResetSnake(); }

SnakeController::~SnakeController() {}

}  // namespace s21
