#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <panel.h>

#include "config.h"
#include "qsoform.h"
#include "qsolist.h"


qsoListComponent *qso_list;


qsoListComponent *newQsoListComponent(void) {
  qsoListComponent *co = (qsoListComponent *)malloc(sizeof(qsoListComponent));
  memset((void *)co->buffer, 0, 4096);

  return co;
}


void initQsoListComponent(qsoListComponent *co) {
  int ii;

  init_pair(QSOLISTCOMPONENT_COLOR_PAIR, COLOR_WHITE, COLOR_BLACK);

  co->window = newwin(LINES - 4, COLS, 0, 0);
  keypad(co->window, TRUE);

  co->panel = new_panel(co->window);
  box(co->window, 0, 0);

  wattron(co->window, COLOR_PAIR(QSOLISTCOMPONENT_COLOR_PAIR) | A_BOLD);
  for (ii = 0; ii < QFFT_MAX; ii++) {
    mvwprintw(co->window, 1, qso_form_field[ii].left + 1, qso_form_field[ii].label);
  }
  wattroff(co->window, COLOR_PAIR(QSOLISTCOMPONENT_COLOR_PAIR) | A_BOLD);

  co->pad = newpad(LINES - 7, COLS - 3);
  touchwin(co->window);
}


void refreshQsoListComponent(qsoListComponent *co) {
  mvwprintw(co->pad, 0, 0, co->buffer);
  prefresh(co->pad, 0, 0, 2, 2, LINES - 6, COLS - 3);
}


void freeQsoListComponent(qsoListComponent *co) {
  delwin(co->pad);

  del_panel(co->panel);

  delwin(co->window);

  free(co);
}


void addQsoListComponentItem(qsoListComponent *co, struct tm *timeinfo,
                             const char *mode, const char *band,
                             const char *callsign, const char *rstsent,
                             const char *rstrcvd) {
  char timestr[32] = { 0 };

  strftime(timestr, sizeof(timestr) - 1, "%Y %b %d %H:%M ", timeinfo);

  sprintf(co->buffer, "%s%s %s %s %s %s %s\n", co->buffer, timestr, mode, band,
          callsign, rstsent, rstrcvd);
}
