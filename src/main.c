#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <form.h>
#include <panel.h>

#include "config.h"
#include "qsoform.h"


int main(void)
{
  qsoFormComponent *qso_form;
  PANEL *panel;
  WINDOW *win, *pad;
  int ch;
  char qsolist[2048] = {0};

  /* Initialize ncurses. */
  initscr();
  start_color();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);

  qso_form = newQsoFormComponent();
  initQsoFormComponent(qso_form);

  win = newwin(20, COLS, 0, 0);
  keypad(win, TRUE);
  panel = new_panel(win);
  box(win, 0, 0);
  pad = newpad(18, COLS - 2);
  touchwin(win);

  while (1) {
    ch = getch();

    /* Refresh UI. */
    refreshQsoFormComponent(qso_form);
    mvwprintw(pad, 0, 0, qsolist);
    update_panels();
    doupdate();
    prefresh(pad, 0, 0, 1, 1, 18, COLS - 2);

    if (ch == ERR) {
      napms(25);
      continue;
    }

    switch (ch) {
    case KEY_F(1):
      goto quit;
      break;
    case 10:
      form_driver(qso_form->form, REQ_VALIDATION);
      sprintf(qsolist,
              "%s%s %s%s%s%s%s\n",
              qsolist,
              field_buffer(qso_form->field[QFFT_TIMESTAMP], 0),
              field_buffer(qso_form->field[QFFT_MODE], 0),
              field_buffer(qso_form->field[QFFT_BAND], 0),
              field_buffer(qso_form->field[QFFT_CALLSIGN], 0),
              field_buffer(qso_form->field[QFFT_RSTSENT], 0),
              field_buffer(qso_form->field[QFFT_RSTRCVD], 0));

      form_driver(qso_form->form, REQ_LAST_FIELD);
      form_driver(qso_form->form, REQ_CLR_FIELD);
      form_driver(qso_form->form, REQ_PREV_FIELD);
      form_driver(qso_form->form, REQ_CLR_FIELD);
      form_driver(qso_form->form, REQ_PREV_FIELD);
      form_driver(qso_form->form, REQ_CLR_FIELD);
      form_driver(qso_form->form, REQ_PREV_FIELD);
      form_driver(qso_form->form, REQ_CLR_FIELD);
      form_driver(qso_form->form, REQ_PREV_FIELD);
      form_driver(qso_form->form, REQ_CLR_FIELD);
      form_driver(qso_form->form, REQ_PREV_FIELD);
      form_driver(qso_form->form, REQ_CLR_FIELD);
      break;
    default:
      processQsoFormComponentInput(qso_form, ch);
      break;
    }

  }

 quit:
  freeQsoFormComponent(qso_form);

  endwin();

  return 0;
}
