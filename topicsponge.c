/* topicsponge.c - Example to print sources as they are discovered.
 * See https://github.com/UltraMessaging/topicsponge */

/*
  Copyright (c) 2023-2024 Informatica Corporation
  Permission is granted to licensees to use or alter this software for any
  purpose, including commercial applications, according to the terms laid
  out in the Software License Agreement.

  This source code example is provided by Informatica for educational
  and evaluation purposes only.

  THE SOFTWARE IS PROVIDED "AS IS" AND INFORMATICA DISCLAIMS ALL WARRANTIES 
  EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY IMPLIED WARRANTIES OF 
  NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR 
  PURPOSE.  INFORMATICA DOES NOT WARRANT THAT USE OF THE SOFTWARE WILL BE 
  UNINTERRUPTED OR ERROR-FREE.  INFORMATICA SHALL NOT, UNDER ANY CIRCUMSTANCES,
  BE LIABLE TO LICENSEE FOR LOST PROFITS, CONSEQUENTIAL, INCIDENTAL, SPECIAL OR 
  INDIRECT DAMAGES ARISING OUT OF OR RELATED TO THIS AGREEMENT OR THE 
  TRANSACTIONS CONTEMPLATED HEREUNDER, EVEN IF INFORMATICA HAS BEEN APPRISED OF 
  THE LIKELIHOOD OF SUCH DAMAGES.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/time.h>

#include "lbm/lbm.h"


/* Simple error handler. */
#define E(lbm_funct_call_) do { \
  int e_ = (lbm_funct_call_); \
  if (e_ == LBM_FAILURE) { \
    fprintf(stderr, "ERROR (%s:%d): %s failed: '%s'\n", __FILE__, __LINE__, #lbm_funct_call_, lbm_errmsg()); \
    exit(1); \
  } \
} while (0)  /* E */


/* See https://github.com/fordsfords/timefmt */
void timefmt(char *buffer, size_t buff_len, char *format)
{
  struct timespec cur_timespec;
  struct tm cur_timestruct;
  char ns_str[16];

  clock_gettime(CLOCK_REALTIME, &cur_timespec);

  if (format[0] == 'L') {
    (void)localtime_r(&cur_timespec.tv_sec, &cur_timestruct);
  }
  else if (format[0] == 'U') {
    (void)gmtime_r(&cur_timespec.tv_sec, &cur_timestruct);
  }
  else { fprintf(stderr, "Format must start with 'L' or 'U'\n"); exit(1); }

  buffer[0] = '\0';  /* Just in case strftime doesn't touch the buffer. */
  (void)strftime(buffer, buff_len, &format[1], &cur_timestruct);

  /* Add factional part (if requested). */

  snprintf(ns_str, sizeof(ns_str), "%09lu", cur_timespec.tv_nsec);  /* Leading zeros. */

  while (*buffer != '\0') {  /* Search buffer for decimal point. */
    if (*buffer == '.') {  /* Found decimal point. */
      int i = 0;  /* Index into ns_str[]. */
      buffer++;  /* Step past the decimal point. */
      while (*buffer == '.') {  /* Overwrite rest of '.' with ns_str. */
        if (ns_str[i] != '\0') {
          *buffer = ns_str[i];
          i++;
        } else {  /* Past end of ns_str; pad with '0'. */
          *buffer = '0';
        }
        buffer++;
      }  /* while '.' */
    }
    else {  /* buffer not pointing at '.' */
      buffer++;
    }
  }  /* while not '\0'. */
}  /* timefmt */


int rcv_handle_src_notify(const char *topic_str, const char *src_str, void *clientd)
{
  char time_buff[100];

  timefmt(time_buff, sizeof(time_buff), "L%F %T.......");
  printf("%s %s %s\n", topic_str, src_str, time_buff);
  fflush(stdout);

  return 0;
}


int main(int argc, char **argv)
{
  lbm_context_attr_t *ctx_attr;
  lbm_src_notify_func_t src_notify;
  lbm_context_t *ctx;
  char time_buff[100];

  timefmt(time_buff, sizeof(time_buff), "L%F %T.......");
  printf("topicsponge started %s\n", time_buff);
  fflush(stdout);

  /* Retrieve current context settings */
  E(lbm_context_attr_create(&ctx_attr));

  /* Set the callback for new source notification */
  src_notify.clientd = NULL;
  src_notify.notifyfunc = rcv_handle_src_notify;
  E(lbm_context_attr_setopt(ctx_attr, "resolver_source_notification_function", &src_notify, sizeof(src_notify)));

  /* Create LBM context */
  E(lbm_context_create(&ctx, ctx_attr, NULL, NULL));

  lbm_context_attr_delete(ctx_attr);

  while (1) { sleep(100); }

  return 0;
}  /* main */
