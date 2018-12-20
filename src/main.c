#include <newt.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"


char message[] = ("This is a pretty long message. It will be displayed "
                  "in a newt textbox, and illustrates how to construct "
                  "a textbox from arbitrary text which may not have "
                  "very good line breaks.\n\n"
                  "Notice how literal \\n characters are respected, and "
                  "may be used to force line breaks and blank lines.");

int main(void)
{
  newtComponent form, text, button;

  newtInit();
  newtCls();

  text = newtTextboxReflowed(1, 1, message, 30, 5, 5, 0);
  button = newtButton(12, newtTextboxGetNumLines(text) + 2, "Ok");

  newtOpenWindow(10, 5, 37,
                 newtTextboxGetNumLines(text) + 7, "Textboxes");

  form = newtForm(NULL, NULL, 0);
  newtFormAddComponents(form, text, button, NULL);

  newtRunForm(form);
  newtFormDestroy(form);
  newtFinished();

  return 0;
}
