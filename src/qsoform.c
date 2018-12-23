#include <stdlib.h>
#include <time.h>

#include <form.h>
#include <panel.h>

#include "config.h"
#include "qsoform.h"
#include "qsolist.h"


qsoFormField qso_form_field[QFFT_MAX] = {
  {
    .type_id = QFFT_TIMESTAMP,
    .label = "Date        Time",
    .width = 17,
    .height = 1,
    .top = 1,
    .left = 1,
    .bgcolor = COLOR_PAIR(QSOFORMPANEL_COLOR_PAIR),
    .fgcolor = COLOR_PAIR(QSOFORMPANEL_COLOR_PAIR),
    .options = O_PUBLIC | O_STATIC | O_VISIBLE,
  },
  {
    .type_id = QFFT_MODE,
    .label = "Mode",
    .width = 5,
    .height = 1,
    .top = 1,
    .left = 20,
    .bgcolor = COLOR_PAIR(QSOFORMPANEL_COLOR_PAIR),
    .fgcolor = COLOR_PAIR(QSOFORMPANEL_COLOR_PAIR),
    .options = O_PUBLIC | O_STATIC | O_VISIBLE,
  },
  {
    .type_id = QFFT_BAND,
    .label = "Band",
    .width = 8,
    .height = 1,
    .top = 1,
    .left = 26,
    .bgcolor = COLOR_PAIR(QSOFORMPANEL_COLOR_PAIR),
    .fgcolor = COLOR_PAIR(QSOFORMPANEL_COLOR_PAIR),
    .options = O_PUBLIC | O_STATIC | O_VISIBLE,
  },
  {
    .type_id = QFFT_CALLSIGN,
    .label = "Callsign",
    .width = 10,
    .height = 1,
    .top = 1,
    .left = 35,
    .bgcolor = COLOR_PAIR(QSOFORMPANEL_COLOR_PAIR),
    .fgcolor = COLOR_PAIR(QSOFORMPANEL_COLOR_PAIR),
    .options = O_PUBLIC | O_STATIC | O_VISIBLE | O_EDIT | O_ACTIVE,
  },
  {
    .type_id = QFFT_RSTSENT,
    .label = "RSTs",
    .width = 4,
    .height = 1,
    .top = 1,
    .left = 46,
    .bgcolor = COLOR_PAIR(QSOFORMPANEL_COLOR_PAIR),
    .fgcolor = COLOR_PAIR(QSOFORMPANEL_COLOR_PAIR),
    .options = O_PUBLIC | O_STATIC | O_VISIBLE | O_EDIT | O_ACTIVE,
  },
  {
    .type_id = QFFT_RSTRCVD,
    .label = "RSTr",
    .width = 4,
    .height = 1,
    .top = 1,
    .left = 51,
    .bgcolor = COLOR_PAIR(QSOFORMPANEL_COLOR_PAIR),
    .fgcolor = COLOR_PAIR(QSOFORMPANEL_COLOR_PAIR),
    .options = O_PUBLIC | O_STATIC | O_VISIBLE | O_EDIT | O_ACTIVE,
  },
};


qsoFormComponent *qso_form;


qsoFormComponent *newQsoFormComponent(void) {
  return (qsoFormComponent *)malloc(sizeof(qsoFormComponent));
}


void initQsoFormComponent(qsoFormComponent *co) {
  int cols, rows, ii;

  init_pair(QSOFORMPANEL_COLOR_PAIR, COLOR_WHITE, COLOR_BLACK);

  for (ii = 0; ii < QFFT_MAX; ii++) {
    co->field[ii] = new_field(qso_form_field[ii].height,
                              qso_form_field[ii].width,
                              qso_form_field[ii].top,
                              qso_form_field[ii].left,
                              0, 0);
    set_field_opts(co->field[ii], qso_form_field[ii].options);
    set_field_back(co->field[ii], qso_form_field[ii].bgcolor);
    set_field_fore(co->field[ii], qso_form_field[ii].fgcolor);
  }

  co->field[QFFT_MAX] = NULL;

  co->form = new_form(co->field);

  scale_form(co->form, &rows, &cols);

  co->window = newwin(rows + 2, COLS, 20, 0);
  keypad(co->window, TRUE);

  box(co->window, 0, 0);

  co->panel = new_panel(co->window);

  set_form_win(co->form, co->window);
  set_form_sub(co->form, derwin(co->window, rows, cols + 1, 1, 1));

  post_form(co->form);

  wattron(co->window, COLOR_PAIR(QSOFORMPANEL_COLOR_PAIR) | A_BOLD);
  for (ii = 0; ii < QFFT_MAX; ii++) {
    mvwprintw(co->window, 1, qso_form_field[ii].left + 1, qso_form_field[ii].label);
  }
  wattroff(co->window, COLOR_PAIR(QSOFORMPANEL_COLOR_PAIR) | A_BOLD);
}


void refreshQsoFormComponent(qsoFormComponent *co) {
  time_t rawtime;
  struct tm *timeinfo;
  char timestr[32] = { 0 };

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(timestr, sizeof(timestr) - 1, "%Y %b %d %H:%M", timeinfo);
  set_field_buffer(co->field[QFFT_TIMESTAMP], 0, timestr);

  set_field_buffer(co->field[QFFT_BAND], 0, "40M");
  set_field_buffer(co->field[QFFT_MODE], 0, "CW");
}


void processQsoFormComponentInput(qsoFormComponent *co, int ch) {
  time_t rawtime;
  struct tm *timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);

  switch (ch) {
    case KEY_RIGHT:
    case '\t':
      form_driver(co->form, REQ_NEXT_FIELD);
      form_driver(co->form, REQ_END_LINE);
      break;
    case KEY_LEFT:
      form_driver(co->form, REQ_PREV_FIELD);
      form_driver(co->form, REQ_END_LINE);
      break;
    case KEY_BACKSPACE:
      form_driver(co->form, REQ_DEL_PREV);
      form_driver(co->form, REQ_END_LINE);
      break;
    case 10:
      // TODO(JS): this should eventually go to the respective forms
      form_driver(co->form, REQ_VALIDATION);

      addQsoListComponentItem(qso_list, timeinfo,
                              field_buffer(co->field[QFFT_MODE], 0),
                              field_buffer(co->field[QFFT_BAND], 0),
                              field_buffer(co->field[QFFT_CALLSIGN], 0),
                              field_buffer(co->field[QFFT_RSTSENT], 0),
                              field_buffer(co->field[QFFT_RSTRCVD], 0));

      form_driver(co->form, REQ_LAST_FIELD);
      form_driver(co->form, REQ_CLR_FIELD);
      form_driver(co->form, REQ_PREV_FIELD);
      form_driver(co->form, REQ_CLR_FIELD);
      form_driver(co->form, REQ_PREV_FIELD);
      form_driver(co->form, REQ_CLR_FIELD);
      form_driver(co->form, REQ_PREV_FIELD);
      form_driver(co->form, REQ_CLR_FIELD);
      form_driver(co->form, REQ_PREV_FIELD);
      form_driver(co->form, REQ_CLR_FIELD);
      form_driver(co->form, REQ_PREV_FIELD);
      form_driver(co->form, REQ_CLR_FIELD);
      break;
    default:
      form_driver(co->form, ch);
      break;
  }
}


void freeQsoFormComponent(qsoFormComponent *co) {
  int ii;

  unpost_form(co->form);

  del_panel(co->panel);

  delwin(co->window);

  free_form(co->form);

  for (ii = 0; ii < QFFT_MAX; ii++) {
    free_field(co->field[ii]);
  }

  free(co);
}
