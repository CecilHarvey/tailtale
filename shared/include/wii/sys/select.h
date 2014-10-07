/* sys/select.h

   netport
*/

#ifndef _SYS_SELECT_H
#define _SYS_SELECT_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Get fd_set, and macros like FD_SET */
#include <sys/types.h>
#include <gctypes.h>

/* Get definition of timeval.  */
#include <sys/time.h>
#include <time.h>

/* Get definition of sigset_t. */
#include <signal.h>

int  pselect(int, fd_set *__restrict__, fd_set *__restrict__, fd_set *__restrict__,
         const struct timespec *__restrict__, const sigset_t *__restrict__);
int  select(int, fd_set *__restrict__, fd_set *__restrict__, fd_set *__restrict__,
         struct timeval *__restrict__);

#ifdef __cplusplus
};
#endif

#endif /* sys/select.h */
