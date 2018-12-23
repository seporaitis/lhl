#ifndef __qsolist_h__
#define __qsolist_h__

#define QSOFORMPANEL_COLOR_PAIR 1


typedef struct qsoListComponent_s {
  PANEL  *panel;
  WINDOW *window;
  WINDOW *pad;
  char    buffer[2048];
} qsoListComponent;


extern qsoListComponent *qso_list;


qsoListComponent *newQsoListComponent(void);
void initQsoListComponent(qsoListComponent *co);
void refreshQsoListComponent(qsoListComponent *co);
void freeQsoListComponent(qsoListComponent *co);

void addQsoListComponentItem(qsoListComponent *co, struct tm *timeinfo,
                             const char *mode, const char *band,
                             const char *callsign, const char *rstsent,
                             const char *rstrcvd);


#endif // __qsolist_h__
