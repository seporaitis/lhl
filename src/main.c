#include <config.h>
#include <ncurses.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
  initscr();
  cbreak();

  keypad(stdscr, TRUE);

  refresh();

  int mx, my;
  getmaxyx(stdscr, mx, my);

  WINDOW *win = newwin(mx, my, 0, 0);
  box(win, 0, 0);
  wrefresh(win);

  getch();

  endwin();

  return 0;
}
