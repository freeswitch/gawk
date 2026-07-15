/* Minimal langinfo.h shim for MinGW-w64, standing in for the LibGw32C
   header the old GnuWin32 builds used.  gawk only calls
   nl_langinfo(CODESET) (in dfa.c and regcomp.c) to check whether the
   codeset is UTF-8; returning "" means "not UTF-8", which matches the
   behavior of the historical Win32 builds.  */
#ifndef _LANGINFO_H
#define _LANGINFO_H

#define CODESET 0
typedef int nl_item;

static __inline__ const char *
nl_langinfo (nl_item item)
{
  (void) item;
  return "";
}

#endif /* _LANGINFO_H */
