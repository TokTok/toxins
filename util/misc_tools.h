#ifndef MISC_TOOLS_H
#define MISC_TOOLS_H

/* Reimplementation of strncasecmp() function from strings.h, as strings.h is
 * POSIX and not portable. Specifically it doesn't exist on MSVC.
 */
int tox_strncasecmp(const char *s1, const char *s2, size_t n);

/* You are responsible for freeing the return value! */
uint8_t *hex_string_to_bin(const char *hex_string);

int cmdline_parsefor_ipv46(int argc, char **argv, uint8_t *ipv6enabled);

/* Sleep function (x = milliseconds). */
void c_sleep(uint32_t x);

#endif /* MISC_TOOLS_H */
