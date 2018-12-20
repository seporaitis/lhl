#include <newt.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"


struct calllog_s {
  newtComponent co;
  unsigned int ctr;
  char buf[1024];
};


void updateCallLog(newtComponent co, void *data) {
  struct calllog_s *callog = (struct calllog_s *)data;
  const char *fmt = "%s20M   CW     20-Dec-18 22:00   %04d  %s        %d  %d  177\n";

  callog->ctr++;
  sprintf(callog->buf, fmt, callog->buf, callog->ctr, "LY2GV", 599, 599);
  newtTextboxSetText(callog->co, callog->buf);
}


int main(void)
{
  newtComponent text;
  unsigned int cols, rows;
  char callsignVal[256] = {0};
  char rstSentVal[256] = {0};
  char rstRcvdVal[256] = {0};

  char buf[1024] = {0};

  newtInit();
  newtCls();

  newtGetScreenSize(&cols, &rows);
  newtPushHelpLine(NULL);

  newtComponent form1 = newtForm(NULL, NULL, 0);
  struct calllog_s callog = {
    .co = newtTextbox(2, 2, cols - 6, 10, NEWT_FLAG_SCROLL),
    .ctr = 0,
    .buf = {0}
  };

  newtTextboxSetColors(callog.co, NEWT_COLORSET_BORDER, NEWT_COLORSET_LABEL);
  newtFormAddComponents(form1, callog.co, NULL);
  newtDrawForm(form1);

  newtComponent form = newtForm(NULL, NULL, 0);
  newtComponent band = newtLabel(2, 14, "20M");
  newtComponent mode = newtLabel(8, 14, "CW");
  newtComponent timestamp = newtLabel(16, 14, "20-Dec-18 22:00");
  newtComponent counter = newtLabel(34, 14, "0002");
  newtComponent callsign = newtEntry(40, 14, "", 10, &callsignVal, 0);
  newtComponent rstSent = newtEntry(52, 14, "599", 10, &rstSentVal, 0);
  newtComponent rstRcvd = newtEntry(60, 14, "599", 10, &rstRcvdVal, 0);
  newtFormAddComponents(form, band, mode, timestamp, counter, callsign, rstSent, rstRcvd, NULL);
  newtComponentAddCallback(rstRcvd, updateCallLog, &callog);
  newtFormRun(form, NULL);

  newtFormDestroy(form);
  newtFinished();

  return 0;
}
