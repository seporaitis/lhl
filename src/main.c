#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <form.h>
#include <ncurses.h>

#include "config.h"


enum {
  FIELD_TIMESTAMP,
  FIELD_MODE,
  FIELD_BAND,
  FIELD_CALLSIGN,
  FIELD_RSTSENT,
  FIELD_RSTRCVD,
  FIELD_MAX,
};


int main(void)
{
  FIELD *field[FIELD_MAX + 1];
  FORM *form;
  int ch;

  /* Initialize ncurses. */
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  /* Initialize the fields. */
  field[FIELD_TIMESTAMP] = new_field(1, 10, 10, 1, 0, 0);
  field[FIELD_MODE] = new_field(1, 10, 10, 12, 0, 0);
  field[FIELD_BAND] = new_field(1, 10, 10, 23, 0, 0);
  field[FIELD_CALLSIGN] = new_field(1, 10, 10, 34, 0, 0);
  field[FIELD_RSTSENT] = new_field(1, 4, 10, 45, 0, 0);
  field[FIELD_RSTRCVD] = new_field(1, 4, 10, 50, 0, 0);
  field[FIELD_MAX] = NULL;

  /* Set field options. */
  set_field_back(field[FIELD_TIMESTAMP], A_UNDERLINE);
  field_opts_off(field[FIELD_TIMESTAMP], O_AUTOSKIP);

  set_field_back(field[FIELD_MODE], A_UNDERLINE);
  field_opts_off(field[FIELD_MODE], O_AUTOSKIP);

  set_field_back(field[FIELD_BAND], A_UNDERLINE);
  field_opts_off(field[FIELD_BAND], O_AUTOSKIP);

  set_field_back(field[FIELD_CALLSIGN], A_UNDERLINE);
  field_opts_off(field[FIELD_CALLSIGN], O_AUTOSKIP);

  set_field_back(field[FIELD_RSTSENT], A_UNDERLINE);
  field_opts_off(field[FIELD_RSTSENT], O_AUTOSKIP);

  set_field_back(field[FIELD_RSTRCVD], A_UNDERLINE);
  field_opts_off(field[FIELD_RSTRCVD], O_AUTOSKIP);

  /* Create a form. */
  form = new_form(field);
  post_form(form);
  refresh();

  while ((ch = getch()) != KEY_F(1)) {
    switch (ch) {
    case KEY_RIGHT:
    case '\t':
      form_driver(form, REQ_NEXT_FIELD);
      form_driver(form, REQ_END_LINE);
      break;
    case KEY_LEFT:
      form_driver(form, REQ_PREV_FIELD);
      form_driver(form, REQ_END_LINE);
      break;
    case KEY_BACKSPACE:
      form_driver(form, REQ_DEL_PREV);
      form_driver(form, REQ_END_LINE);
      break;
    case 10:
      form_driver(form, REQ_VALIDATION);
      mvprintw(0, 1, "Timestamp: '%s'\nMode: '%s'\nBand: '%s'\nCallsign: '%s'\nRSTs: '%s'\nRSTr: '%s'\n",
               field_buffer(field[FIELD_TIMESTAMP], 0),
               field_buffer(field[FIELD_MODE], 0),
               field_buffer(field[FIELD_BAND], 0),
               field_buffer(field[FIELD_CALLSIGN], 0),
               field_buffer(field[FIELD_RSTSENT], 0),
               field_buffer(field[FIELD_RSTRCVD], 0));

      form_driver(form, REQ_LAST_FIELD);
      form_driver(form, REQ_CLR_FIELD);
      form_driver(form, REQ_PREV_FIELD);
      form_driver(form, REQ_CLR_FIELD);
      form_driver(form, REQ_PREV_FIELD);
      form_driver(form, REQ_CLR_FIELD);
      form_driver(form, REQ_PREV_FIELD);
      form_driver(form, REQ_CLR_FIELD);
      form_driver(form, REQ_PREV_FIELD);
      form_driver(form, REQ_CLR_FIELD);
      form_driver(form, REQ_PREV_FIELD);
      form_driver(form, REQ_CLR_FIELD);
      break;
    default:
      form_driver(form, ch);
      break;
    }
  }

  unpost_form(form);
  free_form(form);
  free_field(field[FIELD_TIMESTAMP]);
  free_field(field[FIELD_MODE]);
  free_field(field[FIELD_BAND]);
  free_field(field[FIELD_CALLSIGN]);
  free_field(field[FIELD_RSTSENT]);
  free_field(field[FIELD_RSTRCVD]);

  endwin();

  return 0;
}
