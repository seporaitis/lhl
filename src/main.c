#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <form.h>
#include <ncurses.h>

#include "config.h"


/**
 * QSO FORM
 *
 * Handles the QSO input form.
 */


typedef enum {
  QSOF_TIMESTAMP,
  QSOF_MODE,
  QSOF_BAND,
  QSOF_CALLSIGN,
  QSOF_RSTSENT,
  QSOF_RSTRCVD,
  QSOF_MAX,
} qsoform_field_t;


typedef struct {
  FIELD *field[QSOF_MAX + 1];
  FORM *form;
} qsoform_t;


qsoform_t *new_qsoform() {
  qsoform_t *obj = (qsoform_t *)malloc(sizeof(qsoform_t));

  obj->field[QSOF_TIMESTAMP] = new_field(1, 10, 10, 1, 0, 0);
  set_field_back(obj->field[QSOF_TIMESTAMP], A_UNDERLINE);
  field_opts_off(obj->field[QSOF_TIMESTAMP], O_AUTOSKIP);

  obj->field[QSOF_MODE] = new_field(1, 10, 10, 12, 0, 0);
  set_field_back(obj->field[QSOF_MODE], A_UNDERLINE);
  field_opts_off(obj->field[QSOF_MODE], O_AUTOSKIP);

  obj->field[QSOF_BAND] = new_field(1, 10, 10, 23, 0, 0);
  set_field_back(obj->field[QSOF_BAND], A_UNDERLINE);
  field_opts_off(obj->field[QSOF_BAND], O_AUTOSKIP);

  obj->field[QSOF_CALLSIGN] = new_field(1, 10, 10, 34, 0, 0);
  set_field_back(obj->field[QSOF_CALLSIGN], A_UNDERLINE);
  field_opts_off(obj->field[QSOF_CALLSIGN], O_AUTOSKIP);

  obj->field[QSOF_RSTSENT] = new_field(1, 4, 10, 45, 0, 0);
  set_field_back(obj->field[QSOF_RSTSENT], A_UNDERLINE);
  field_opts_off(obj->field[QSOF_RSTSENT], O_AUTOSKIP);

  obj->field[QSOF_RSTRCVD] = new_field(1, 4, 10, 50, 0, 0);
  set_field_back(obj->field[QSOF_RSTRCVD], A_UNDERLINE);
  field_opts_off(obj->field[QSOF_RSTRCVD], O_AUTOSKIP);

  obj->field[QSOF_MAX] = NULL;

  obj->form = new_form(obj->field);

  return obj;
}


void free_qsoform(qsoform_t *obj) {
  free_form(obj->form);

  free_field(obj->field[QSOF_TIMESTAMP]);
  free_field(obj->field[QSOF_MODE]);
  free_field(obj->field[QSOF_BAND]);
  free_field(obj->field[QSOF_CALLSIGN]);
  free_field(obj->field[QSOF_RSTSENT]);
  free_field(obj->field[QSOF_RSTRCVD]);

  free(obj);
}



int main(void)
{
  qsoform_t *qsoform;
  int ch;

  /* Initialize ncurses. */
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  qsoform = new_qsoform();
  post_form(qsoform->form);
  refresh();

  while ((ch = getch()) != KEY_F(1)) {
    switch (ch) {
    case KEY_RIGHT:
    case '\t':
      form_driver(qsoform->form, REQ_NEXT_FIELD);
      form_driver(qsoform->form, REQ_END_LINE);
      break;
    case KEY_LEFT:
      form_driver(qsoform->form, REQ_PREV_FIELD);
      form_driver(qsoform->form, REQ_END_LINE);
      break;
    case KEY_BACKSPACE:
      form_driver(qsoform->form, REQ_DEL_PREV);
      form_driver(qsoform->form, REQ_END_LINE);
      break;
    case 10:
      form_driver(qsoform->form, REQ_VALIDATION);
      mvprintw(0, 1, "Timestamp: '%s'\nMode: '%s'\nBand: '%s'\nCallsign: '%s'\nRSTs: '%s'\nRSTr: '%s'\n",
               field_buffer(qsoform->field[QSOF_TIMESTAMP], 0),
               field_buffer(qsoform->field[QSOF_MODE], 0),
               field_buffer(qsoform->field[QSOF_BAND], 0),
               field_buffer(qsoform->field[QSOF_CALLSIGN], 0),
               field_buffer(qsoform->field[QSOF_RSTSENT], 0),
               field_buffer(qsoform->field[QSOF_RSTRCVD], 0));

      form_driver(qsoform->form, REQ_LAST_FIELD);
      form_driver(qsoform->form, REQ_CLR_FIELD);
      form_driver(qsoform->form, REQ_PREV_FIELD);
      form_driver(qsoform->form, REQ_CLR_FIELD);
      form_driver(qsoform->form, REQ_PREV_FIELD);
      form_driver(qsoform->form, REQ_CLR_FIELD);
      form_driver(qsoform->form, REQ_PREV_FIELD);
      form_driver(qsoform->form, REQ_CLR_FIELD);
      form_driver(qsoform->form, REQ_PREV_FIELD);
      form_driver(qsoform->form, REQ_CLR_FIELD);
      form_driver(qsoform->form, REQ_PREV_FIELD);
      form_driver(qsoform->form, REQ_CLR_FIELD);
      break;
    default:
      form_driver(qsoform->form, ch);
      break;
    }
  }

  unpost_form(qsoform->form);
  free_qsoform(qsoform);

  endwin();

  return 0;
}
