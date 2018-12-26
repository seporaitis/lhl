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

  return co;
}


void initQsoListComponent(qsoListComponent *co) {
  int ii;

  co->cursor = 0;
  co->numitems = 0;

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
  int ii, idx = 0, totalsize = 0;
  char *buffer;

  for (ii = 0; ii < co->numitems; ii++) {
    totalsize += co->item[ii].length + 1;
  }

  buffer = malloc(sizeof(char) * totalsize + 2);
  memset(buffer, 0, totalsize + 2);

  for (ii = 0; ii < co->numitems; ii++) {
    memcpy(&buffer[idx], co->item[ii].buffer, co->item[ii].length);
    idx += co->item[ii].length;
    buffer[idx] = '\n';
    idx++;
  }

  mvwprintw(co->pad, 0, 0, buffer);
  prefresh(co->pad, co->cursor, 0, 2, 2, LINES - 6, COLS - 3);

  free(buffer);
}


void processQsoListComponentInput(qsoListComponent *co, int ch) {
  switch (ch) {
  case KEY_UP:
    co->cursor--;
    if (co->cursor < 0) co->cursor = 0;
    break;
  case KEY_DOWN:
    co->cursor++;
    if (co->cursor >= co->numitems) co->cursor = co->numitems;
    break;
  }
}


void freeQsoListComponent(qsoListComponent *co) {
  int ii;

  delwin(co->pad);

  del_panel(co->panel);

  delwin(co->window);

  for (ii = 0; ii < co->numitems; ii++) {
    free(co->item[ii].buffer);
  }

  free(co->item);

  free(co);
}


void addQsoListComponentItem(qsoListComponent *co, struct tm *timeinfo,
                             const char *mode, const char *band,
                             const char *callsign, const char *rstsent,
                             const char *rstrcvd) {
  co->item = realloc(co->item, sizeof(qsoListItem) * (co->numitems + 1));

  char timestr[32] = { 0 };
  strftime(timestr, sizeof(timestr) - 1, "%Y %b %d %H:%M ", timeinfo);

  co->item[co->numitems].buffer = malloc(sizeof(char) * (COLS - 3));
  memset(co->item[co->numitems].buffer, 0, sizeof(char) * (COLS - 3));
  snprintf(co->item[co->numitems].buffer, COLS,
           "%s %s %s %s %s %s",
           timestr, mode, band, callsign, rstsent, rstrcvd);
  co->item[co->numitems].length = strlen(co->item[co->numitems].buffer);

  co->numitems++;
}
