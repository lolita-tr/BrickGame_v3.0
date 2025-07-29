#include "../../inc/tetris/tetris_frontend.h"
#include "../../inc/tetris/fsm.h"
#include "../../inc/game_common.h"

/** @file */

/**
 * @brief Starts the Tetris game.
 *
 * This function initiates the main game loop and performs necessary
 * cleanup after the game ends by calling endwin() to restore terminal
 * settings. It sets up the game environment and manages the overall
 * game flow.
 */

void start_tetris_game() {
  game_loop();
  endwin();
}

/**
 * @brief Main game loop for the Tetris game.
 *
 * This function handles the primary game loop, managing game state updates,
 * user input, and rendering. It initializes the game window, processes user
 * inputs to control the tetromino, updates the game state, and refreshes the
 * display accordingly. The loop continues until the game is either paused or
 * ended. It also handles the timing for tetromino movement and game speed.
 * Upon game termination, it cleans up allocated resources.
 */

void game_loop() {
  srand(time(NULL));
  generate_figure();

  GameInfo *game_info = get_game_info();
  Tetromino *tet = set_tetromino(game_info);
  int key = 0;

  WINDOW *startwin = newwin(20 * 3 + 1, 20 * 2 + 1, 1, 1);
  start_screen(startwin, tet, game_info);

  clock_t lastTime, currentTime;
  lastTime = clock();

  WINDOW *gamewin = newwin(20 * 3 + 1, 20 * 2 + 1, 1, 1);
  WINDOW *pausewin = newwin(20 * 3 + 1, 20 * 2 + 1, 1, 1);
  while (game_info->pause != QUIT && game_info->pause != LOSED) {
    werase(gamewin);

    currentTime = clock();
    key = getch();

    user_input(tet, game_info, key);

    refresh_game(gamewin, /*game_info->field,*/ tet, game_info);
    if (game_info->pause == STARTED) {
      if ((currentTime - lastTime) > (CLOCKS_PER_SEC / 250)) {
        move_tetromino_down_one_row(tet, game_info);
        lastTime = currentTime;
      }
      if (tet->is_placed) game_update(tet, game_info);

    } else if (game_info->pause == PAUSED) {
      print_pause_sceen(pausewin);
      lastTime = currentTime;
    }
    usleep(game_info->speed);
  }
  game_over_scree(gamewin, game_info);
  free_tetromino(tet);
  free_game(game_info);
}

/**
 * @brief Reads user input and assigns the corresponding action to the given
 * pointer.
 *
 * @param[in] action pointer to a Signals enum variable, which will be set to
 * the corresponding action.
 * @param[in] sign the user input character.
 */
void user_input(Tetromino *tet, GameInfo *game_info, int sign) {
  UserAction action = handle_user_input(sign);
  get_signal(tet, game_info, action);
}

/**
 * @brief Initializes the ncurses library settings.
 *
 * Sets up the ncurses environment by initializing the screen, hiding the
 * cursor, disabling echoing of characters, and enabling keypad input and
 * non-blocking mode.
 */
void init_ncurses() {
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


/**
 * @brief Prints the playing field on a given window.
 *
 * Draws the border of the playing field using the print_rectangle
 * function with the specified dimensions. The field is defined
 * by the constants FIELD_HEIGHT and FIELD_WIDTH.
 *
 * @param[in] win the window to draw the field on.
 */
// void print_field(WINDOW *win) {
//   print_rectangle(win, 0, FIELD_HEIGHT, 0, FIELD_WIDTH + 18);
// }

/**
 * @brief Prints the info bar on a given window.
 *
 * Prints the level, score, and high score on the given window.
 * The info bar is defined by the constants FIELD_WIDTH and FIELD_HEIGHT.
 *
 * @param[in] win the window to draw the bar on.
 * @param[in] game_info the game info structure containing the game info.
 */
void print_info_bar(WINDOW *win/*, GameInfo *game_info*/) {
  //print_info_bar(win, game_info);
  mvwprintw(win, 16, 19, "Next:");
}

/**
 * @brief Renders the current state of the play field and active tetromino.
 *
 * Iterates through the play field and draws each cell on the specified window.
 * Filled cells are drawn with a specific color and character, while empty cells
 * are represented by a different character. Additionally, it renders the active
 * tetromino on the field and displays the next tetromino in the info section.
 *
 * @param[in] win the window to draw the play field on.
 * @param[in] play_field a 2D array representing the state of the play field.
 * @param[in] tet the current active tetromino to be drawn on the field.
 * @param[in] game_info the game info structure containing additional game data.
 */
void draw_play_field(WINDOW *win, /*int **play_field,*/ Tetromino *tet,
                      GameInfo *game_info) {
  // for (int i = 0; i < FIELD_H; i++) {
  //   for (int j = 0; j < FIELD_W; j++) {
  //     if (/*play_field[i][j] == 1*/ game_info->field[i][j] == 1) {
  //       wattron(win, COLOR_PAIR(3));
  //       mvwaddch(win, i + 1, j + 1, '0');
  //       wattroff(win, COLOR_PAIR(3));
  //     } else {
  //       mvwaddch(win, i + 1, j + 1, '.');
  //     }
  //   }
  // }
  print_play_field(win, game_info);
  draw_tetromino_on_field(win, tet);
  draw_next_tetromino_in_info(win, game_info);
  wrefresh(win);
}

/**
 * @brief Draws the active tetromino on the game field.
 *
 * This function iterates through the active tetromino's figure array and draws
 * each filled cell on the specified window at the correct position, applying a
 * specific color pair for visual distinction. The tetromino is drawn based on
 * its current coordinates, and only the occupied cells in the figure are
 * rendered.
 *
 * @param[in] win The window to draw the tetromino on.
 * @param[in] tet The active tetromino to be drawn.
 */
void draw_tetromino_on_field(WINDOW *win, Tetromino *tet) {
  for (int y = 0; y < MAX_FIGURE_SIZE; y++) {
    for (int x = 0; x < MAX_FIGURE_SIZE; x++) {
      if (tet->figure[y][x]) {
        wattron(win, COLOR_PAIR(3));
        mvwaddch(win, tet->coord.y + 1 + y, tet->coord.x + 1 + x, '#');
        wattroff(win, COLOR_PAIR(3));
      }
    }
  }
}

/**
 * @brief Draws the next tetromino on the information bar.
 *
 * This function iterates through the next tetromino's figure array and draws
 * each filled cell on the specified window at the correct position, applying a
 * specific color pair for visual distinction. The tetromino is drawn below the
 * game field, and only the occupied cells in the figure are rendered.
 *
 * @param[in] win The window to draw the next tetromino on.
 * @param[in] game_info The game information structure, which contains the next
 * tetromino's figure array.
 */
void draw_next_tetromino_in_info(WINDOW *win, GameInfo *game_info) {
  for (int y = 0; y < MAX_FIGURE_SIZE; y++) {
    for (int x = 0; x < MAX_FIGURE_SIZE; x++) {
      if (game_info->next[y][x]) {
        wattron(win, COLOR_PAIR(3));
        mvwprintw(win, y + 17, FIELD_W + 9 + x, "#");
        wattroff(win, COLOR_PAIR(3));
      } else {
        mvwprintw(win, y + 17, FIELD_W + 9 + x, " ");
      }
    }
  }
}

/**
 * @brief Updates the game window with the latest game state.
 *
 * This function is a wrapper for updating the game window. It first clears the
 * window by drawing the game field, then draws the current state of the play
 * field and active tetromino, and finally draws the information bar with the
 * next tetromino and game information. The window is then updated with
 * wrefresh().
 *
 * @param[in] win The window to update.
 * @param[in] play_field The 2D array representing the play field.
 * @param[in] tet The active tetromino to be drawn.
 * @param[in] game_info The game information structure containing additional
 * game data.
 */
void refresh_game(WINDOW *win, /*int **play_field,*/ Tetromino *tet,
                  GameInfo *game_info) {
  print_field(win);
  draw_play_field(win, /*play_field,*/ tet, game_info);
  print_info_bar(win, game_info);
  wrefresh(win);
}

/**
 * @brief Prints the start screen on a given window.
 *
 * Prints the start screen on the given window by drawing a rectangle and
 * printing the string "START SCREEN" on it. The window is then updated with
 * wrefresh().
 *
 * @param[in] win The window to draw the start screen on.
 */
void print_start_sceen(WINDOW *win) {
  print_rectangle(win, 0, FIELD_HEIGHT, 0, FIELD_WIDTH + 18);
  mvwprintw(win, 4, 12, "TETRIS");
  mvwprintw(win, 6, 6, "Press ENTER to Start");
  mvwprintw(win, 12, 6, "-> Right");
  mvwprintw(win, 13, 6, "<- Left");
  mvwprintw(win, 14, 6, "Space - Rotate");
  mvwprintw(win, 16, 6, "P - Pause");
  mvwprintw(win, 17, 6, "Q - Quit");
  wrefresh(win);
}

/**
 * @brief Prints the pause screen on a given window.
 *
 * Prints the pause screen on the given window by printing the string "PAUSE"
 * on it. The window is then updated with wrefresh().
 *
 * @param[in] win The window to draw the pause screen on.
 */
void print_pause_sceen(WINDOW *win) {
  mvwprintw(win, 1, 1, "PAUSE");
  wrefresh(win);
}

/**
 * @brief Prints the game over screen on a given window.
 *
 * Prints the game over screen on the given window by drawing a rectangle and
 * printing the string "GAME OVER! SEE YOU LATER!" on it. The window is then
 * updated with wrefresh().
 *
 * @param[in] win The window to draw the game over screen on.
 */
void print_game_over_screen(WINDOW *win, GameInfo *game_info) {
  print_rectangle(win, 0, FIELD_HEIGHT, 0, FIELD_WIDTH + 18);
  if (game_info->score == game_info->high_score &&
      game_info->high_score >= 100) {
    mvwprintw(win, 8, 6, "CONGRATULATIONS!!!");
    mvwprintw(win, 10, 5, "YOU SET A NEW RECORD");
    mvwprintw(win, 12, 6, "YOUR SCORE: %d", game_info->high_score);
  } else {
    mvwprintw(win, 10, 2, "GAME OVER! SEE YOU LATER;)");
  }
  wrefresh(win);
}

/**
 * @brief Displays the start screen of the game.
 *
 * Creates a new window and calls print_start_sceen() on it. The window is
 * then refreshed with wrefresh(). The function waits until the user presses
 * the Enter key and then deletes the window and clears the screen.
 */

void start_screen(WINDOW *startwin, Tetromino *tet, GameInfo *game_info) {
  int key = 0;

  while (game_info->pause == NOT_STARTED) {
    key = getch();
    print_rectangle(startwin, 0, FIELD_HEIGHT, 0, FIELD_WIDTH + 18);
    print_start_sceen(startwin);
    user_input(tet, game_info, key);
    wrefresh(startwin);
  }
  endwin();
}

/**
 * @brief Displays the game over screen of the game.
 *
 * Creates a new window and calls print_game_over_screen() on it. The window is
 * then refreshed with wrefresh(). The function waits until the user presses
 * any key and then deletes the window and clears the screen.
 *
 * @param[in] win The window to draw the game over screen on.
 */
void game_over_scree(WINDOW *win, GameInfo *game_info) {
  WINDOW *game_over_win = newwin(20 * 3 + 1, 20 * 2 + 1, 1, 1);

  werase(win);
  wrefresh(win);

  nodelay(stdscr, FALSE);
  print_game_over_screen(game_over_win, game_info);

  getch();
}
