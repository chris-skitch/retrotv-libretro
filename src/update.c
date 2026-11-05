#include "update.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int update_parse_tag(const char *json, char *tag, size_t n)
{
  if (!json || !n) return -1;
  const char *p = strstr(json, "\"tag_name\"");
  if (!p) return -1;
  p += strlen("\"tag_name\"");
  while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') p++;
  if (*p++ != ':') return -1;
  while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') p++;
  if (*p++ != '"') return -1;
  size_t o = 0;
  for (; *p && *p != '"'; p++) {
    unsigned char c = (unsigned char)*p;
    if (!isalnum(c) && c != '.' && c != '_' && c != '-') return -1;
    if (o + 1 >= n) return -1;
    tag[o++] = (char)c;
  }
  if (*p != '"' || !o) return -1;
  tag[o] = 0;
  return 0;
}

/* the dotted numbers, up to a "-" or the end */
static int cmp_core(const char **pa, const char **pb)
{
  const char *a = *pa, *b = *pb;
  int c = 0;
  while (!c && ((*a && *a != '-') || (*b && *b != '-'))) {
    long x = isdigit((unsigned char)*a) ? strtol(a, (char **)&a, 10) : 0;
    long y = isdigit((unsigned char)*b) ? strtol(b, (char **)&b, 10) : 0;
    if (x != y) c = x < y ? -1 : 1;
    while (*a && *a != '.' && *a != '-') a++;
    while (*b && *b != '.' && *b != '-') b++;
    if (*a == '.') a++;
    if (*b == '.') b++;
  }
  *pa = a;
  *pb = b;
  return c;
}

/* "beta.2" against "beta.10" or "rc1": numbers by value, the rest by character */
static int cmp_suffix(const char *a, const char *b)
{
  while (*a && *b) {
    if (isdigit((unsigned char)*a) && isdigit((unsigned char)*b)) {
      long x = strtol(a, (char **)&a, 10), y = strtol(b, (char **)&b, 10);
      if (x != y) return x < y ? -1 : 1;
      continue;
    }
    if (*a != *b) return (unsigned char)*a < (unsigned char)*b ? -1 : 1;
    a++;
    b++;
  }
  return *a ? 1 : *b ? -1 : 0;
}

int update_cmp(const char *a, const char *b)
{
  if (*a == 'v' || *a == 'V') a++;
  if (*b == 'v' || *b == 'V') b++;
  int c = cmp_core(&a, &b);
  if (c) return c;
  if (!*a || !*b) return !*a && !*b ? 0 : *a ? -1 : 1;   /* 0.4.0-beta.1 is older than 0.4.0 */
  return cmp_suffix(a + 1, b + 1);
}

int update_sums_has(const char *sums, const char *file)
{
  size_t fl = strlen(file);
  for (const char *l = sums; l && *l;) {
    const char *e = strchr(l, '\n');
    size_t len = e ? (size_t)(e - l) : strlen(l);
    while (len && (l[len - 1] == '\r' || l[len - 1] == ' ')) len--;
    if (len > fl + 1 && !memcmp(l + len - fl, file, fl) && (l[len - fl - 1] == ' ' || l[len - fl - 1] == '*'))
      return 1;
    l = e ? e + 1 : NULL;
  }
  return 0;
}
