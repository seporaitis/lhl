#include <config.h>
#include <curses.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "hamlog.h"
#include "terminal.h"


static terminal_t terminal;


void
term_init(void) {
  /* modify stdin terminals attributes to allow Ctrl-Q/S key recognition */
  tcgetattr(STDIN_FILENO, &terminal.oldt);
  terminal.newt = terminal.oldt;
  terminal.newt.c_iflag &= ~(IXON);
  tcsetattr(STDIN_FILENO, TCSANOW, &terminal.newt);

  /* initialize curses screen */
  initscr();

  /* get and check terminal dimensions */
  getmaxyx(stdscr, terminal.ymax, terminal.xmax);
  if ((terminal.ymax < 25) || (terminal.xmax < 80)) {
    endwin();
    exit(1);
  }

  /* initialize colors */
  if (!has_colors() || (start_color() == ERR)) {
    endwin();
    exit(1);
  }

  refresh();

  /* disable line buffering and read each key */
  cbreak();

  /* do not echo characters as they are typed */
  noecho();

  /* enable keypad */
  keypad(stdscr, TRUE);
}


void
term_setup_color(void) {
  init_pair(COLOR_BLACK, COLOR_BLACK, COLOR_WHITE);
  init_pair(C_HEADER, COLOR_GREEN, COLOR_YELLOW);
  init_pair(COLOR_RED, COLOR_WHITE, COLOR_RED);
  init_pair(C_WINDOW, COLOR_CYAN, COLOR_WHITE);
  init_pair(C_LOG, COLOR_WHITE, COLOR_BLACK);
  init_pair(C_DUPE, COLOR_WHITE, COLOR_MAGENTA);
  init_pair(C_INPUT, COLOR_BLUE, COLOR_YELLOW);
  init_pair(C_BORDER, COLOR_WHITE, COLOR_BLACK);
}


void
term_close(void) {
  endwin();
}
