/*
* Project   COOS - simple COoperative OS
* Author    A.Kouznetsov
* Rev       1.1 dated 9/3/2010
* Target    PIC24FJ64GA002

Redistribution and use in source and binary forms, with or without modification, are permitted.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* ------------------------------------------------------- */
/* includes                                                */
/* ------------------------------------------------------- */

#include "p24fj64ga002.h"     /* target */
#include  "coos.h"

/* ------------------------------------------------------- */
/* function prototypes                                     */
/* ------------------------------------------------------- */
void COOS_task0(void);
void COOS_task1(void);
void COOS_task2(void);
void COOS_task3(void);

/* ------------------------------------------------------- */
/* vars                                                    */
/* ------------------------------------------------------- */

jmp_buf       COOS_main_context;
jmp_buf       COOS_task_context[4];       /* store environment for 4 tasks */
uchar         COOS_task_no;
int           COOS_task_delay[4];         /* task delay in 100 us ticks, task stopped if value is negative */
/* WARNING: for 8-bit microcontrollers ensure atomic updates of COOS_task_delay[] by either using 8-bit long
variables or by interrupt disable/enable functions when changing COOS_task_delay[] outside interrupt servicing routine */

int dly, cnt; /* those variables must be global too */

/* ------------------------------------------------------- */
/*           Main                                          */
/* ------------------------------------------------------- */
int main(void)
{
  int   res;
  /* ------------------------ */
  /* Init hardware            */
  /* ------------------------ */
  CLKDIVbits.RCDIV = 0;
  PADCFG1 = 0xFF;             /* Make analog pins digital */

  LATB = 0x0;
  TRISB = 0x313;

  TMR1 = 0;                   /* Clear timer 1 */
  PR1 = 1475;                 /* Interrupt every 100 us (actually 100.03) */
  IFS0bits.T1IF = 0;          /* Clear interrupt flag */
  IEC0bits.T1IE = 1;          /* Set interrupt enable bit */
  T1CON = 0x8000;             /* Fosc/42, 1:1 prescale, start TMR1 */

  /* ------------------------ */
  /* Init tasks               */
  /* ------------------------ */
  for (COOS_task_no=0; COOS_task_no<4; COOS_task_no++)
  {
    res = setjmp(COOS_main_context);
    if (res == 0)
    {
      switch (COOS_task_no)
      {
        case 0 : COOS_task0();
        case 1 : COOS_task1();
        case 2 : COOS_task2();
        case 3 : COOS_task3();
      }
    }
    else
      COOS_task_delay[COOS_task_no] = --res;
  }
  /* ------------------------ */
  /* Scheduler loop           */
  /* ------------------------ */
  COOS_task_no = 0;
  for (;;)
  {
    if (COOS_task_delay[COOS_task_no] == 0)           /* if task is active */
    {
      res = setjmp(COOS_main_context);                /* set return point and get delay value from the task */
      if (res == 0)                                   /* after setting return point */
        longjmp(COOS_task_context[COOS_task_no], 1);  /* invoke task */
      else                                            /* after returning from task */
        COOS_task_delay[COOS_task_no] = --res;        /* set task delay (negative delay - task stopped) */
    }
    COOS_task_no++;         /* round-robin */
    COOS_task_no &= 3;      /* limit to 4 tasks */
  }
return 0;
}
/* ------------------------------------------------------- */
/*      Tasks											   */
/* ------------------------------------------------------- */
/* DELAY() must be used in the outer loop, it cannot be used in functions */
void COOS_task0(void)
{
  dly = 0x20; 				/* start with very fast LED toggle rate */
  for (;;)
  {
    DELAY(dly);
    LATB ^= 0x1000;       	/* toggle LED */
    if (dly > 0x2000)
      dly = 0x200;
    else
      dly += (dly >> 4);  	/* dly = 1.0625*dly */
  }
}
/* ------------------------------------------------------- */
void COOS_task1(void)
{
  LATB |= 0x2000;			/* LED off */
  DELAY(STOP);				/* task stopped */
}
/* ------------------------------------------------------- */
void COOS_task2(void)
{
  cnt = 0;
  for (;;)
  {
    if  (LATB & 0x4000)
    {
      if (cnt > 5)
        DELAY(5000);		/* long pause */
      else
        DELAY(2000);		/* short pause */
      if (++cnt > 11) 
        cnt = 0;
    }
    else
      DELAY(500);			/* short flash */
    LATB ^= 0x4000;       	/* toggle LED */
  }
}
/* ------------------------------------------------------- */
void COOS_task3(void)
{
  DELAY(0);
  for (;;)
  {
    DELAY(8000);
    LATB ^= 0x8000;       /* toggle LED */
  }
}

/* ------------------------------------------------------- */
/*      Interrupt                                          */
/* ------------------------------------------------------- */
/* every 100 us */
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{
  uchar i;
  IFS0bits.T1IF = 0;      /* clear interrupt flag */
  /* decrement task delays */
  for (i=0; i<4; i++)
  {
    if (COOS_task_delay[i] > 0)
      COOS_task_delay[i]--;
  }
}


