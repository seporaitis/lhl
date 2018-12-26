#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <panel.h>

#include "config.h"
#include "qsoform.h"
#include "qsolist.h"


#define QSOLISTCOMPONENT_PAD_ROWS (LINES - 6)
#define QSOLISTCOMPONENT_PAD_COLS (COLS - 3)


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

  co->pad = newpad(1024, QSOLISTCOMPONENT_PAD_COLS);
  touchwin(co->window);
}


void refreshQsoListComponent(qsoListComponent *co) {
  int ii, totalsize = 0;
  char *buffer, *ptr;

  for (ii = 0; ii < co->numitems; ii++) {
    totalsize += co->item[ii].length + 1;
  }

  buffer = malloc(sizeof(char) * totalsize + 2);
  memset(buffer, 0, sizeof(char) * totalsize + 2);
  ptr = buffer;

  for (ii = 0; ii < co->numitems; ii++) {
    memcpy(ptr, co->item[ii].buffer, co->item[ii].length);
    ptr += co->item[ii].length;
    *ptr = '\n';
    ptr++;
  }

  mvwprintw(co->pad, 0, 0, buffer);
  prefresh(co->pad, co->cursor, 0,
           2, 2, QSOLISTCOMPONENT_PAD_ROWS, QSOLISTCOMPONENT_PAD_COLS);

  free(buffer);
}


void processQsoListComponentInput(qsoListComponent *co, int ch) {
  switch (ch) {
  case KEY_UP:
    if (co->cursor > 0) co->cursor--;
    break;
  case KEY_DOWN:
    if (co->cursor + QSOLISTCOMPONENT_PAD_ROWS - 1 < co->numitems) co->cursor++;
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

  co->item[co->numitems].buffer = malloc(sizeof(char) * QSOLISTCOMPONENT_PAD_COLS);
  memset(co->item[co->numitems].buffer, 0, sizeof(char) * QSOLISTCOMPONENT_PAD_COLS);
  snprintf(co->item[co->numitems].buffer, COLS,
           "%s %s %s %s %s %s",
           timestr, mode, band, callsign, rstsent, rstrcvd);
  co->item[co->numitems].length = strlen(co->item[co->numitems].buffer);

  co->numitems++;
  if (co->numitems - QSOLISTCOMPONENT_PAD_ROWS + 1 > 0) {
    co->cursor = (co->numitems - QSOLISTCOMPONENT_PAD_ROWS + 1);
  }
}
