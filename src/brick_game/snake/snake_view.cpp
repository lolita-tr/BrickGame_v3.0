#include "../../inc/snake/snake_view.h"

#include "../../inc/snake/snake.h"

/** @file */

namespace s21 {
/**
 * @brief Constructs a SnakeView object.
 *
 * @param controller Reference to a SnakeController object used to manage the
 * game logic and interaction.
 */

SnakeView::SnakeView(s21::SnakeController &controller)
    : controller_(controller) {}

/**
 * @brief Starts the snake game by initializing the game window and handling the
 * game loop.
 *
 * This function creates two windows: one for the start screen and another for
 * the game. The start screen prompts the user to press "Enter" to start the
 * game. Once started, the function enters a game loop where it continuously
 * handles user input and updates the game state until the game is quit.
 */

void SnakeView::StartSnakeGame() {
  WINDOW *startwin = newwin(20 * 3 + 1, 20 * 2 + 1, 1, 1);

  while (controller_.snake_.GetPauseState() == NOT_STARTED) {
    PrintRectangle(startwin, 0, FIELD_HEIGHT, 0, FIELD_WIDTH + 18);
    mvwprintw(startwin, 10, 6, "Press Enter to start");
    HandelInput();
    wrefresh(startwin);
  }
  endwin();

  WINDOW *gamewin = newwin(20 * 3 + 1, 20 * 2 + 1, 1, 1);
  while (controller_.snake_.GetPauseState() != QUIT) {
    werase(gamewin);
    HandelInput();
    RefreshGame(gamewin, controller_.snake_.GetGameInfo()/*, controller_.snake_*/);
    if (controller_.snake_.GetPauseState() == STARTED) {
      controller_.UpdateCurrentState();
    }
  }
}

/**
 * @brief Handles user input for controlling the snake game.
 *
 * This function captures keyboard input and translates it into game actions
 * by sending commands to the SnakeController. The input keys include arrow keys
 * for movement, 'p' or 'P' to pause, 'q' or 'Q' to terminate the game, 'Enter'
 * to start, and the space bar for additional actions. The function does not
 * return any value.
 */

void SnakeView::HandelInput() {
  int ch = getch();
  switch (ch) {
    case KEY_UP:
      controller_.UserInput(Up, false);
      break;

    case KEY_DOWN:
      controller_.UserInput(Down, false);
      break;

    case KEY_LEFT:
      controller_.UserInput(Left, false);
      break;

    case KEY_RIGHT:
      controller_.UserInput(Right, false);
      break;

    case 'p':
    case 'P':
      controller_.UserInput(Pause, false);
      break;

    case 'q':
    case 'Q':
      controller_.UserInput(Terminate, false);
      break;
    case '\n':
      controller_.UserInput(Start, false);
      break;
    case ' ':
      controller_.UserInput(Action, true);
      break;
  }
}

/**
 * @brief Prints a rectangle on a given window.
 *
 * Prints a rectangle on a given window with the specified coordinates. The
 * rectangle is drawn using the ncurses extended characters for lines and
 * corners.
 *
 * @param[in] win the window to draw the rectangle on.
 * @param[in] top_y the y-coordinate of the top of the rectangle.
 * @param[in] bottom_y the y-coordinate of the bottom of the rectangle.
 * @param[in] left_x the x-coordinate of the left of the rectangle.
 * @param[in] right_x the x-coordinate of the right of the rectangle.
 */
void PrintRectangle(WINDOW *win, int top_y, int bottom_y, int left_x,
                    int right_x) {
  mvwaddch(win, top_y, left_x, ACS_ULCORNER);

  int i = left_x + 1;
  while (i < right_x) {
    mvwaddch(win, top_y, i++, ACS_HLINE);
  }
  mvwaddch(win, top_y, i, ACS_URCORNER);

  for (int i = top_y + 1; i < bottom_y; i++) {
    mvwaddch(win, i, left_x, ACS_VLINE);
    mvwaddch(win, i, right_x, ACS_VLINE);
  }

  mvwaddch(win, bottom_y, left_x, ACS_LLCORNER);
  i = left_x + 1;
  while (i < right_x) {
    mvwaddch(win, bottom_y, i++, ACS_HLINE);
  }
  mvwaddch(win, bottom_y, i, ACS_LRCORNER);
}

/**
 * @brief Prints the game field border.
 *
 * Prints the game field border on a given window.
 *
 * @param[in] win the window to draw the game field on.
 */
void SnakeView::PrintField(WINDOW *win) {
  PrintRectangle(win, 0, FIELD_HEIGHT, 0, FIELD_WIDTH);
}

/**
 * @brief Prints the information bar on the game window.
 *
 * Renders the information bar displaying the current level, score, and
 * high score of the snake game. Each section of the info bar is enclosed
 * in a rectangle and displays its respective label and value. The info bar
 * is positioned to the right of the game field.
 *
 * @param[in] win the window to draw the information bar on.
 */

void SnakeView::PrintInfoBar(WINDOW *win) {
  PrintRectangle(win, 0, 4, FIELD_WIDTH + 2, FIELD_WIDTH + 18);
  mvwprintw(win, 1, 19, "Level:");
  mvwprintw(win, 3, 21, "%d", controller_.snake_.GetLevel());

  PrintRectangle(win, 5, 9, FIELD_WIDTH + 2, FIELD_WIDTH + 18);
  mvwprintw(win, 6, 19, "Score:");
  mvwprintw(win, 8, 16, "%6d", controller_.snake_.GetScore());

  PrintRectangle(win, 10, 14, FIELD_WIDTH + 2, FIELD_WIDTH + 18);
  mvwprintw(win, 11, 16, "High Score:");
  mvwprintw(win, 13, 16, "%6d", controller_.snake_.GetHighScore());

  PrintRectangle(win, 15, FIELD_HEIGHT, FIELD_WIDTH + 2, FIELD_WIDTH + 18);
}

/**
 * @brief Refreshes the game window with the current game state.
 *
 * This function updates the game window by redrawing the game field,
 * information bar, apple, snake, and any additional messages. It ensures
 * that the latest game state is visually represented. The window is refreshed
 * at the end to display all updates.
 *
 * @param[in] win the window to refresh with the current game state.
 * @param[in] game_info the current game information, including the apple's
 * position.
 * @param[in] snake the current snake object, providing its position and state.
 */

void SnakeView::RefreshGame(WINDOW *win, const GameInfo &game_info/*, Snake &snake*/) {
  PrintField(win);
  PrintInfoBar(win);
  DrawGameField(win, game_info);
  PrintOtherMessage(win);
  wrefresh(win);
}

/**
 * @brief Draws the entire game field including snake and apple.
 *
 * This function draws all game elements on the game window by scanning
 * the field array and drawing appropriate symbols for each cell type.
 *
 * @param[in] win the window to draw the game field on.
 * @param[in] game_info the current game information containing the field state.
 */
void SnakeView::DrawGameField(WINDOW *win, const GameInfo &game_info) {
  for (int y = 0; y < FIELD_H; ++y) {
    for (int x = 0; x < FIELD_W; ++x) {
      switch (game_info.field[x][y]) {
        case 0: 
          wattron(win, COLOR_PAIR(1));
          mvwaddch(win, y+1, x+1, '.');
          wattroff(win, COLOR_PAIR(1));
          break;
        case 1: // Тело змейки
          mvwaddch(win, y+1, x+1, 'o');
          break;
        case 2: // Голова змейки
          mvwaddch(win, y+1, x+1, '0');
          break;
        case 3: // Яблоко
          wattron(win, COLOR_PAIR(3));
          mvwaddch(win, y+1, x+1, '@');
          wattroff(win, COLOR_PAIR(3));
          break;
        default: 
          break;
      }
    }
  }
}



/**
 * @brief Prints additional messages on the game window.
 *
 * This function prints specific messages on the game window depending on
 * the current pause state of the game. The messages are printed at a fixed
 * position on the window.
 *
 * @param[in] win the window to print the message on.
 */
void SnakeView::PrintOtherMessage(WINDOW *win) {
  if (controller_.snake_.GetPauseState() == PAUSED) {
    mvwprintw(win, 1, 1, "PAUSE");
  } else if (controller_.snake_.GetPauseState() == WIN) {
    mvwprintw(win, 1, 1, "You won!!!");
  } else if (controller_.snake_.GetPauseState() == LOSED) {
    mvwprintw(win, 1, 1, "You lost!");
  }
}

}  // namespace s21