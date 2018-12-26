#ifndef __qsolist_h__
#define __qsolist_h__

#define QSOLISTCOMPONENT_COLOR_PAIR 2


typedef struct qsoListItem_s {
  int   length;
  char *buffer;
} qsoListItem;


typedef struct qsoListComponent_s {
  PANEL       *panel;
  WINDOW      *window;
  WINDOW      *pad;
  int          cursor;
  int          numitems;
  qsoListItem *item;
} qsoListComponent;


extern qsoListComponent *qso_list;


qsoListComponent *newQsoListComponent(void);
void initQsoListComponent(qsoListComponent *co);
void refreshQsoListComponent(qsoListComponent *co);
void processQsoListComponentInput(qsoListComponent *co, int ch);
void freeQsoListComponent(qsoListComponent *co);

void addQsoListComponentItem(qsoListComponent *co, struct tm *timeinfo,
                             const char *mode, const char *band,
                             const char *callsign, const char *rstsent,
                             const char *rstrcvd);


#endif // __qsolist_h__
