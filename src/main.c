#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <form.h>
#include <panel.h>

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
  WINDOW *win;
  PANEL *panel;
} qsoform_t;


qsoform_t *new_qsoform() {
  qsoform_t *obj = (qsoform_t *)malloc(sizeof(qsoform_t));
  int cols, rows;

  obj->field[QSOF_TIMESTAMP] = new_field(1, 17, 1, 1, 0, 0);
  set_field_back(obj->field[QSOF_TIMESTAMP], COLOR_PAIR(1));
  set_field_fore(obj->field[QSOF_TIMESTAMP], COLOR_PAIR(1));
  field_opts_off(obj->field[QSOF_TIMESTAMP], O_AUTOSKIP | O_ACTIVE | O_EDIT);

  obj->field[QSOF_MODE] = new_field(1, 5, 1, 20, 0, 0);
  set_field_back(obj->field[QSOF_MODE], COLOR_PAIR(1));
  set_field_fore(obj->field[QSOF_MODE], COLOR_PAIR(1));
  field_opts_off(obj->field[QSOF_MODE], O_AUTOSKIP | O_ACTIVE | O_EDIT);
  set_field_buffer(obj->field[QSOF_MODE], 0, "CW");

  obj->field[QSOF_BAND] = new_field(1, 8, 1, 26, 0, 0);
  set_field_back(obj->field[QSOF_BAND], COLOR_PAIR(1));
  set_field_fore(obj->field[QSOF_BAND], COLOR_PAIR(1));
  field_opts_off(obj->field[QSOF_BAND], O_AUTOSKIP | O_ACTIVE | O_EDIT);
  set_field_buffer(obj->field[QSOF_BAND], 0, "40M");

  obj->field[QSOF_CALLSIGN] = new_field(1, 10, 1, 35, 0, 0);
  set_field_back(obj->field[QSOF_CALLSIGN], A_UNDERLINE);
  field_opts_off(obj->field[QSOF_CALLSIGN], O_AUTOSKIP);

  obj->field[QSOF_RSTSENT] = new_field(1, 4, 1, 46, 0, 0);
  set_field_back(obj->field[QSOF_RSTSENT], A_UNDERLINE);
  field_opts_off(obj->field[QSOF_RSTSENT], O_AUTOSKIP);

  obj->field[QSOF_RSTRCVD] = new_field(1, 4, 1, 51, 0, 0);
  set_field_back(obj->field[QSOF_RSTRCVD], A_UNDERLINE);
  field_opts_off(obj->field[QSOF_RSTRCVD], O_AUTOSKIP);

  obj->field[QSOF_MAX] = NULL;

  obj->form = new_form(obj->field);

  scale_form(obj->form, &rows, &cols);

  obj->win = newwin(rows + 2, COLS, 20, 0);
  keypad(obj->win, TRUE);

  obj->panel = new_panel(obj->win);

  set_form_win(obj->form, obj->win);
  set_form_sub(obj->form, derwin(obj->win, rows, cols + 1, 1, 1));

  box(obj->win, 0, 0);

  post_form(obj->form);

  wattron(obj->win, COLOR_PAIR(2) | A_BOLD);
  mvwprintw(obj->win, 1, 2, "Date");
  mvwprintw(obj->win, 1, 14, "Time");
  mvwprintw(obj->win, 1, 21, "Mode");
  mvwprintw(obj->win, 1, 27, "Band");
  mvwprintw(obj->win, 1, 36, "Callsign");
  mvwprintw(obj->win, 1, 47, "Sent");
  mvwprintw(obj->win, 1, 52, "Rcvd");
  wattroff(obj->win, COLOR_PAIR(2) | A_BOLD);

  return obj;
}


void free_qsoform(qsoform_t *obj) {
  unpost_form(obj->form);

  delwin(obj->win);

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
  char buf[24] = {0};
  time_t rawtime;
  struct tm *timeinfo;

  /* Initialize ncurses. */
  initscr();
  start_color();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);

  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  //bkgd(COLOR_PAIR(1));

  qsoform = new_qsoform();

  while (1) {
    ch = getch();

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buf, sizeof(buf) - 1, "%Y %b %d %H:%M", timeinfo);
    set_field_buffer(qsoform->field[QSOF_TIMESTAMP], 0, buf);

    if (ch == ERR) {
      napms(25);
      continue;
    }

    switch (ch) {
    case KEY_F(1):
      goto quit;
      break;
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
      mvprintw(0, 0, "Timestamp: '%s'\nMode: '%s'\nBand: '%s'\nCallsign: '%s'\nRSTs: '%s'\nRSTr: '%s'\n",
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

    /* Refresh UI. */
    update_panels();
    doupdate();
  }

 quit:
  free_qsoform(qsoform);

  endwin();

  return 0;
}
