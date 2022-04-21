#ifndef FMT_DHMS
#define FMT_DHMS

#if defined(__cplusplus)
extern "C" {
#endif

extern int fmt_dhms(int s, char *str, size_t len);
extern int fmt_dhms_compact(int s, char *str, size_t len);

#if defined(__cplusplus)
}
#endif

#endif
