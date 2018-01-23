#ifndef TERMINAL_H
#define TERMINAL_H

#include <termios.h>

typedef struct {
  struct termios oldt, newt;
  int xmax, ymax;
} terminal_t;


void term_init(void);
void term_setup_color(void);
void term_close();

#endif // TERMINAL_H
