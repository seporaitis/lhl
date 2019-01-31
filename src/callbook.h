#ifndef __callbook_h__
#define __callbook_h__

#include <sqlite3.h>


typedef struct callbookComponent_s {
  sqlite3 *database;
} callbookComponent;


extern callbookComponent *callbook;


callbookComponent *newCallbookComponent(void);
void initCallbookComponent(callbookComponent *co);
void freeCallbookComponent(callbookComponent *co);

//void addCallbookComponentItem(callbookComponent *co, struct tm *timeinfo,
//                              const char *mode, const char *band,
//                              const char *callsign, const char *rstsent,
//                              const char *rstrcvd);


#endif // __callbook_h__
