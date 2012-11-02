#define HAVE_CHSIZE 1
#define HAVE_FCNTL_H 1
#define HAVE_FT_BITMAP_SIZE_Y_PPEM 1
#define HAVE_FT_GET_NEXT_CHAR 1
#define HAVE_FT_GET_PS_FONT_INFO 1
#define HAVE_FT_HAS_PS_GLYPH_NAMES 1
#define HAVE_FT_SELECT_SIZE 1
//#define HAVE_FT_GET_BDF_PROPERTY 1
#define HAVE_INTTYPES_H 1
#define HAVE_MEMMOVE 1

#define HAVE_MEMORY_H 1
#define HAVE_MEMSET 1
#define HAVE_RAND 1
#define HAVE_STDINT_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRCHR 1
#define HAVE_STRINGS_H 1
#define HAVE_STRING_H 1
#define HAVE_STRRCHR 1
#define HAVE_STRTOL 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_VPRINTF 1
#define STDC_HEADERS 1
#define USE_ICONV 0

#define FLEXIBLE_ARRAY_MEMBER 1

// Don't include mingw unistd.h
#define _UNISTD_H

#define VERSION "2.10.1"

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE

#define FC_DEFAULT_FONTS "WINDOWSFONTDIR"
#define FC_CACHEDIR "LOCAL_APPDATA_FONTCONFIG_CACHE"

#undef UNICODE
#undef _UNICODE
#define MBCS
#define _MBCS

#define F_OK 0
#define S_ISREG(x) (((x) & S_IFREG) == S_IFREG)

#ifndef __cplusplus
#define inline __inline
#endif
