#include <config.h>
#include <ncurses.h>
#include <panel.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "hamlog.h"
#include "terminal.h"


static WINDOW *win;
static PANEL *panel;


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
