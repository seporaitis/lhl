#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <form.h>
#include <panel.h>

#include "config.h"
#include "qsoform.h"
#include "qsolist.h"


void initializeNcurses(void) {
  initscr();
  start_color();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
}


void refreshNcurses(void) {
  update_panels();
  doupdate();
}


void uninitializeNcurses(void) {
  endwin();
}


int main(void)
{
  int ch;

  initializeNcurses();

  qso_form = newQsoFormComponent();
  initQsoFormComponent(qso_form);

  qso_list = newQsoListComponent();
  initQsoListComponent(qso_list);

  while (1) {
    ch = getch();

    /* Refresh UI. */
    refreshQsoFormComponent(qso_form);
    refreshQsoListComponent(qso_list);
    refreshNcurses();

    if (ch == ERR) {
      napms(25);
      continue;
    }

    switch (ch) {
    case KEY_F(1):
      goto quit;
      break;
    default:
      processQsoFormComponentInput(qso_form, ch);
      break;
    }

  }

 quit:
  freeQsoFormComponent(qso_form);
  freeQsoListComponent(qso_list);

  uninitializeNcurses();

  return 0;
}
