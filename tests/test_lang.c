#include <stdio.h>
#include <string.h>

#include "../src/lang.h"

static int fails;
#define CHECK(cond) do { if (!(cond)) { fprintf(stderr, "FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); fails++; } } while (0)

int main(void)
{
  CHECK(lang_get() == LANG_EN);
  CHECK(!strcmp(T("NO SIGNAL"), "NO SIGNAL"));                     /* English: the key itself */
  lang_set(LANG_ES);
  CHECK(!strcmp(T("NO SIGNAL"), "SIN SE\xd1" "AL"));               /* Latin-1 for the TV font */
  CHECK(!strcmp(TU("NO SIGNAL"), "SIN SE\xc3\x91" "AL"));          /* UTF-8 for the frontend */
  CHECK(T("NO SIGNAL") == T("NO SIGNAL"));                         /* made once */
  CHECK(!strcmp(T("not in the table"), "not in the table"));
  char buf[64];
  snprintf(buf, sizeof(buf), T("SCREEN %d"), 2);
  CHECK(!strcmp(buf, "PANTALLA 2"));
  CHECK(!strcmp(lang_day(3), "MI\xc9") && !strcmp(lang_month(0), "ENE"));
  CHECK(!strcmp(lang_day(9), "DOM"));                              /* out of range: no crash */
  lang_set(99);
  CHECK(lang_get() == LANG_EN && !strcmp(lang_day(1), "MON"));
  CHECK(lang_parse("es") == LANG_ES && lang_parse("fr") == LANG_EN && lang_parse(NULL) == LANG_EN);
  CHECK(!strcmp(lang_code(LANG_ES), "es") && !strcmp(lang_code(LANG_EN), "en"));
  if (fails) return 1;
  printf("test_lang: all passed\n");
  return 0;
}
