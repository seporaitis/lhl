#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <panel.h>

#include "config.h"
#include "qsolist.h"


qsoListComponent *qso_list;


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


void addQsoListComponentItem(qsoListComponent *co, struct tm *timeinfo,
                             const char *mode, const char *band,
                             const char *callsign, const char *rstsent,
                             const char *rstrcvd) {
  char timestr[32] = { 0 };

  strftime(timestr, sizeof(timestr) - 1, "%Y %b %d %H:%M ", timeinfo);

  sprintf(co->buffer, "%s%s%s%s%s%s%s\n", co->buffer, timestr, mode, band,
          callsign, rstsent, rstrcvd);
}
