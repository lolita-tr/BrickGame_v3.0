#include "../../inc/snake/snake.h"
#include "../../inc/snake/snake_controller.h"
#include "../../inc/snake/snake_view.h"
#include "../../inc/tetris/tetris.h"
#include "../../inc/game_common.h"

/** @file */

/**
 * @brief Main function of console Brick Game application.
 *
 * Initializes ncurses library, and enters main game loop.
 *
 * In the loop, program waits for user input. Depending on user choice,
 * program either starts a game of Snake or Tetris, or quits the program.
 *
 * Program also handles user input errors and invalid game states.
 *
 * @return 0 on success, 1 on error.
 */
int main() {
  int choosenOption = 0;
  s21::InitNcurses();

  while (choosenOption != -1) {
    s21::HandleInputMenu(&choosenOption);
    s21::DrawMenuScreen(choosenOption);
  }
  if (choosenOption == -1) {
    endwin();
  }
  return 0;
}

namespace s21 {

/**
 * @brief Draws the main menu screen.
 *
 * Creates a window with the options "Snake", "Tetris", and "Exit".
 * The choosen_point parameter selects which option is highlighted.
 *
 * @param choosen_point The currently selected option (0, 1, or 2).
 */
void DrawMenuScreen(int choosen_point) {
  WINDOW *menuwin = newwin(20 * 3 + 1, 20 * 2 + 1, 1, 1);
  print_rectangle(menuwin, 0, FIELD_HEIGHT, 0, FIELD_WIDTH + 18);
  switch (choosen_point) {
    case 0:
      wattron(menuwin, COLOR_PAIR(3));
      mvwprintw(menuwin, 4, 12, "Snake");
      wattroff(menuwin, COLOR_PAIR(3));
      mvwprintw(menuwin, 6, 12, "Tetris");
      mvwprintw(menuwin, 8, 12, "Exit");
      wrefresh(menuwin);
      break;
    case 1:
      mvwprintw(menuwin, 4, 12, "Snake");
      wattron(menuwin, COLOR_PAIR(3));
      mvwprintw(menuwin, 6, 12, "Tetris");
      wattroff(menuwin, COLOR_PAIR(3));
      mvwprintw(menuwin, 8, 12, "Exit");
      wrefresh(menuwin);
      break;
    default:
      mvwprintw(menuwin, 4, 12, "Snake");
      mvwprintw(menuwin, 6, 12, "Tetris");
      wattron(menuwin, COLOR_PAIR(3));
      mvwprintw(menuwin, 8, 12, "Exit");
      wattroff(menuwin, COLOR_PAIR(3));
      wrefresh(menuwin);
      break;
  }
}

/**
 * @brief Handles user input in the main menu.
 *
 * Listens for user input and moves the selection in the main menu accordingly.
 * The selection is stored in the choosen_point parameter.
 *
 * The user can move the selection with the up/down arrow keys, and select
 * the highlighted option by pressing the enter key.
 *
 * @param choosen_point The currently selected option (0, 1, or 2).
 */
void HandleInputMenu(int *choosen_point) {
  int ch = getch();
  switch (ch) {
    case KEY_UP:
      (*choosen_point)--;
      *choosen_point < 0 ? *choosen_point = *choosen_point + 3 : 0;
      break;

    case KEY_DOWN:
      (*choosen_point)++;
      *choosen_point = *choosen_point % 3;
      break;

    case '\n':
      StartChoosenGame(choosen_point);
      break;
  }
}

/**
 * @brief Starts the game selected by the user.
 *
 * Checks the user's selection and starts either the snake game or the tetris
 * game. If the user selects an invalid option, the value of choosen_option is
 * set to -1.
 *
 * @param choosen_option The user's selection (0 for snake, 1 for tetris, or 2
 * for exit).
 */
void StartChoosenGame(int *choosen_option) {
  if (*choosen_option == 0) {
    Snake game;
    SnakeController controller(game);
    SnakeView view(controller);
    view.StartSnakeGame();
  } else if (*choosen_option == 1) {
    start_tetris_game();
  } else {
    *choosen_option = -1;
  }
}

/**
 * @brief Initializes the ncurses library.
 *
 * Initializes the ncurses library settings needed for the game to run.
 * Sets the window to be non-blocking and to not echo user input.
 * Sets the cursor to be invisible.
 */
void InitNcurses() {
  initscr();
  cbreak();
  start_color();
  init_pair(3, COLOR_RED, COLOR_BLACK);
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  scrollok(stdscr, TRUE);
  curs_set(0);
}

}  // namespace s21