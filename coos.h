/*
 * Project   COOS - simple COoperative OS
 * Author    A.Kouznetsov
 * Rev       1.1 dated 26/3/2010

Redistribution and use in source and binary forms, with or without modification, are permitted.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* ------------------------------------------------------- */
#ifndef __COOS_H
#define __COOS_H

/* ------------------------------------------------------- */
/* Common includes                                         */
/* ------------------------------------------------------- */

#include <setjmp.h>

/* ------------------------------------------------------- */
/* Types                                                   */
/* ------------------------------------------------------- */
typedef unsigned char uchar;
typedef signed   char schar;
typedef unsigned int  uint;
typedef unsigned long ulong;

/* ------------------------------------------------------- */
/* Global var                                              */
/* ------------------------------------------------------- */

extern jmp_buf  COOS_main_context;
extern jmp_buf  COOS_task_context[4];    /* store environment for 4 tasks */
extern uchar    COOS_task_no;

/* ------------------------------------------------------- */
/* Macros                                                  */
/* ------------------------------------------------------- */
#define STOP -2
#define DELAY(__delay)  if (!setjmp(COOS_task_context[COOS_task_no])) {longjmp(COOS_main_context, __delay+1);} else

/* ------------------------------------------------------- */
#endif /* __COOS_H */
