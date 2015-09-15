/*
 * fuzztest.c - simple testbench for fuzzing regex
 *
 * compile libtre using afl-gcc (e.g. CC=afl-gcc ./configure)
 * build this testbench and then give the resulting binary to afl-fuzz
 *
 * Author: Anthony Weems
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <time.h>

#define MAXBUF 1024

const char *MATCH_TARGET = "Base Definitions volume of IEEE Std 1003.1-2001";

/*
 * http://pubs.opengroup.org/onlinepubs/007904975/functions/regcomp.html
 */
int match(const char *string, char *pattern)
{
    int status;
    regex_t re;

    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) {
        return(0);
    }
    status = regexec(&re, string, (size_t) 0, NULL, 0);
    regfree(&re);
    if (status != 0) {
        return(0);
    }
    return(1);
}

int main(int argc, char **argv)
{
  char regex[MAXBUF];
  size_t len;

  /* read from afl-fuzz */
  len = read(0, regex, MAXBUF - 1);
  regex[len] = 0;

  /* do the actual work */
  printf("%d\n", match(MATCH_TARGET, regex));

  return 0;
}
