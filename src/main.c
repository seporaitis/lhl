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


int main(void)
{
  qsoFormComponent *qso_form;
  qsoListComponent *qso_list;
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
    case 10:
      // TODO(JS): this should eventually go to the respective forms
      form_driver(qso_form->form, REQ_VALIDATION);
      sprintf(qso_list->buffer,
              "%s%s %s%s%s%s%s\n",
              qso_list->buffer,
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
