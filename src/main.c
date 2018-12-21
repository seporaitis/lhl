#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

#include "config.h"


int main(void)
{
  initscr();
  printw("Hello world.\n");
  refresh();
  getch();
  endwin();

  return 0;
}
