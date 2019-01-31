#include <stdio.h>
#include <stdlib.h>

#include <sqlite3.h>

#include "config.h"
#include "callbook.h"


callbookComponent *callbook;


callbookComponent *newCallbookComponent(void) {
  return (callbookComponent *)malloc(sizeof(callbookComponent));
}


void initCallbookComponent(callbookComponent *co) {
  int rc;

  rc = sqlite3_open("callbook.sqlite3", &co->database);
  if (rc != 0) {
    fprintf(stderr, "Can't open callbook: %s", sqlite3_errmsg(co->database));
    sqlite3_close(co->database);
    exit(1);
  }
}


void freeCallbookComponent(callbookComponent *co) {
  sqlite3_close(co->database);

  free(co);
}
