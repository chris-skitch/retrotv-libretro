/* lang.h - RetroTV's own words in English or Spanish (System Settings >
 * Language). The code is written in English with each string wrapped in T();
 * lang.c holds the translations, keyed by the English text.
 * scripts/check-lang.py (run by make check) makes sure every T("...") and
 * N_("...") string has a translation with the same printf conversions. */
#ifndef RETROTV_LANG_H
#define RETROTV_LANG_H

enum { LANG_EN, LANG_ES, LANG_COUNT };

/* Marks a string that is translated later, by its content (T(buf)): text a
 * pure module returns, for example. */
#define N_(s) (s)

void lang_set(int lang);
int lang_get(void);
/* "en" or "es"; lang_parse gives LANG_EN for anything unknown */
const char *lang_code(int lang);
int lang_parse(const char *code);

/* en in the current language as Latin-1 text for osd_text (en itself when
 * English or untranslated). The pointer stays valid until the program ends. */
const char *T(const char *en) __attribute__((format_arg(1)));
/* the same in UTF-8, for the frontend's menus (core options) */
const char *TU(const char *en) __attribute__((format_arg(1)));

/* day (0 = Sunday) and month (0 = January) abbreviations in capitals, Latin-1 */
const char *lang_day(int wday);
const char *lang_month(int mon);

#endif
