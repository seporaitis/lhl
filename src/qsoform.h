#ifndef __qsoform_h__
#define __qsoform_h__

#include <form.h>


#define QSOFORMCOMPONENT_COLOR_PAIR 1


typedef enum qsoFormFieldType_e {
  QFFT_TIMESTAMP,
  QFFT_MODE,
  QFFT_BAND,
  QFFT_CALLSIGN,
  QFFT_RSTSENT,
  QFFT_RSTRCVD,
  QFFT_MAX,
} qsoFormFieldType;


typedef struct qsoFormField_s {
  qsoFormFieldType  type_id;
  const char       *label;
  int             width;
  int             height;
  int             top;
  int             left;
  chtype            bgcolor;
  chtype            fgcolor;
  Field_Options     options;
} qsoFormField;


typedef struct qsoFormComponent_s {
  FIELD  *field[QFFT_MAX + 1];
  FORM   *form;
  WINDOW *window;
  PANEL  *panel;
} qsoFormComponent;


extern qsoFormField qso_form_field[QFFT_MAX];
extern qsoFormComponent *qso_form;


qsoFormComponent *newQsoFormComponent(void);
void initQsoFormComponent(qsoFormComponent *co);
void refreshQsoFormComponent(qsoFormComponent *co);
void processQsoFormComponentInput(qsoFormComponent *co, int ch);
void freeQsoFormComponent(qsoFormComponent *co);


#endif // __qsoform_h__
