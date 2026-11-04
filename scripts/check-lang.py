#!/usr/bin/env python3
"""Check RetroTV's translations (src/lang.c) against the strings the code uses.

Keys are the English strings in T("..."), TU("...") and N_("...") calls, and
the names and descriptions in src/options.c's tables (translated with TU when
they are registered). Every key needs a translation with the same printf
conversions in the same order. The phone page (src/webui.html) is checked the
same way: its t("..."), data-t texts, aria-labels and placeholders against its
ES dictionary, with the same {placeholders}. Exits 1 on a problem; with -v also
lists Spanish strings longer than their English (to check on screen).
"""
import html
import glob
import json
import re
import sys

STR = r'"((?:[^"\\]|\\.)*)"'


def unescape(s):
    return bytes(s, "utf-8").decode("unicode_escape").encode("latin-1").decode("utf-8")


def code_keys():
    keys = {}
    for path in sorted(glob.glob("src/*.c")):
        if path in ("src/lang.c", "src/webui.c"):
            continue
        text = open(path, encoding="utf-8").read()
        for m in re.finditer(r'\b(?:T|TU|N_)\(\s*' + STR + r'\s*\)', text):
            keys.setdefault(unescape(m.group(1)), path)
    text = open("src/options.c", encoding="utf-8").read()
    for m in re.finditer(r'\{\s*"(?!retrotv_language"|language")[a-z_]+",\s*' + STR + r',\s*' + STR, text):
        for k in (m.group(1), m.group(2)):    # category or option: name, description
            keys.setdefault(unescape(k), "src/options.c")
    body = text[text.index("static const opt_def OPTS[]"):]
    for m in re.finditer(r'\{\s*"[^"]*",\s*' + STR + r'\s*\}', body):   # { "value", "label" }
        label = unescape(m.group(1))
        if label not in ("English", "Espanol"):
            keys.setdefault(label, "src/options.c")
    return keys


def table():
    text = open("src/lang.c", encoding="utf-8").read()
    body = text[text.index("ES[] = {"):text.index("};", text.index("ES[] = {"))]
    out = {}
    for m in re.finditer(r'\{\s*' + STR + r',\s*' + STR + r'\s*\}', body):
        en, es = unescape(m.group(1)), unescape(m.group(2))
        if en in out:
            print(f"lang.c: {en!r} is listed twice")
            out["\0dup"] = ""
        out[en] = es
    return out


def convs(s):
    return re.findall(r"%[-+ #0]*[0-9*]*(?:\.[0-9*]+)?(?:l|ll|z|h)?[a-zA-Z%]", s)


def webui():
    text = open("src/webui.html", encoding="utf-8").read()
    js = lambda v: json.loads('"' + v + '"')
    keys = set()
    for m in re.finditer(r'\bt\(\s*' + STR, text):
        keys.add(js(m.group(1)))
    for m in re.finditer(r'<[^>]*\bdata-t\b[^>]*>([^<]*)<', text):
        keys.add(html.unescape(m.group(1)))
    for m in re.finditer(r'(?:aria-label|placeholder)="([^"]*)"', text):
        keys.add(html.unescape(m.group(1)))
    body = text[text.index("const ES = {"):text.index("};", text.index("const ES = {"))]
    tr = {js(a): js(b) for a, b in re.findall(STR + r':\s*' + STR, body)}
    bad = False
    for k in sorted(keys - set(tr)):
        print(f"src/webui.html: no Spanish for {k!r}")
        bad = True
    for k in sorted(set(tr) - keys):
        print(f"src/webui.html: {k!r} isn't used any more")
        bad = True
    for k in keys & set(tr):
        if sorted(re.findall(r"\{\w+\}", k)) != sorted(re.findall(r"\{\w+\}", tr[k])):
            print(f"src/webui.html: {k!r}: placeholders differ")
            bad = True
    return bad


def main():
    verbose = "-v" in sys.argv
    keys, tr = code_keys(), table()
    bad = "\0dup" in tr
    for k, where in sorted(keys.items()):
        if not k.strip():
            continue
        if k not in tr:
            print(f"{where}: no Spanish for {k!r}")
            bad = True
        elif convs(k) != convs(tr[k]):
            print(f"lang.c: {k!r}: conversions {convs(k)} vs {convs(tr[k])}")
            bad = True
        elif verbose and len(tr[k]) > len(k):
            print(f"longer ({len(k)} -> {len(tr[k])}): {k!r} -> {tr[k]!r}")
    for k in sorted(set(tr) - set(keys) - {"\0dup"}):
        print(f"lang.c: {k!r} isn't used any more")
        bad = True
    for k, v in tr.items():
        try:
            v.encode("latin-1")
        except UnicodeEncodeError:
            print(f"lang.c: {k!r}: the translation has characters the TV font lacks")
            bad = True
    bad = webui() or bad
    sys.exit(1 if bad else 0)


main()
