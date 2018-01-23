#include <config.h>
#include <ncurses.h>
#include <panel.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "hamlog.h"


static struct termios oldt, newt;
static int xmax, ymax;
static WINDOW *win;
static PANEL *panel;


void
term_init(void) {
  /* modify stdin terminals attributes to allow Ctrl-Q/S key recognition */
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_iflag &= ~(IXON);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  /* initialize curses screen */
  initscr();

  /* get and check terminal dimensions */
  getmaxyx(stdscr, ymax, xmax);
  if ((ymax < 25) || (xmax < 80)) {
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
wnicebox(WINDOW *win, int y, int x, int height, int width, char *boxname) {
  height += 1;
  width += 1;

  wattrset(win, COLOR_PAIR(C_BORDER) | A_BOLD);

  mvwaddch(win, y, x, ACS_ULCORNER);
  whline(win, ACS_HLINE, width);
  mvwaddch(win, y, x + width, ACS_URCORNER);
  mvwaddch(win, y + height, x, ACS_LLCORNER);
  whline(win, ACS_HLINE, width);
  mvwaddch(win, y + height, x + width, ACS_LRCORNER);
  mvwvline(win, y + 1, x + width, ACS_VLINE, height - 1);
  mvwvline(win, y + 1, x, ACS_VLINE, height - 1);
  mvwprintw(win, y, x + 2, boxname);
}


void
term_close(void) {
  endwin();
}


int main(int argc, char *argv[])
{
  term_init();
  term_setup_color();

  win = newwin(11, 39, 1, 41);
  panel = new_panel(win);
  show_panel(panel);
  top_panel(panel);

  wnicebox(win, 0, 0, 12, 38, "HELP");

  wrefresh(win);

  getch();

  term_close();

  return 0;
}
