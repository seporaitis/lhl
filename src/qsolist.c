#include <stdlib.h>
#include <string.h>

#include <panel.h>

#include "config.h"
#include "qsolist.h"


qsoListComponent *newQsoListComponent(void) {
  qsoListComponent *co = (qsoListComponent *)malloc(sizeof(qsoListComponent));
  memset((void *)co->buffer, 0, 2048);

  return co;
}


void initQsoListComponent(qsoListComponent *co) {
  co->window = newwin(20, COLS, 0, 0);
  keypad(co->window, TRUE);

  co->panel = new_panel(co->window);
  box(co->window, 0, 0);
  co->pad = newpad(18, COLS - 2);
  touchwin(co->window);
}


void refreshQsoListComponent(qsoListComponent *co) {
  mvwprintw(co->pad, 0, 0, co->buffer);
  prefresh(co->pad, 0, 0, 1, 1, 18, COLS - 2);
}


void freeQsoListComponent(qsoListComponent *co) {
  delwin(co->pad);

  del_panel(co->panel);

  delwin(co->window);

  free(co);
}
