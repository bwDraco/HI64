/******************************************************************************/
/* HI64 System Benchmark                                                      */
/* 64-bit benchmark derived from Hierarchical INTegration (HINT) originally   */
/* developed at Ames Laboratory, U.S. Department of Energy                    */
/*                                                                            */
/* Portions Copyright (C) 1994 Iowa State University Research Foundation, Inc.*/
/* Portions Copyright (C) 2003 Moritz Franosch                                */
/* Portions Copyright (C) 2014 Brian "DragonLord" Wong                        */
/*                                                                            */
/* This program is free software; you can redistribute it and/or modify       */
/* it under the terms of the GNU General Public License as published by       */
/* the Free Software Foundation.  You should have received a copy of the      */
/* GNU General Public License along with this program; if not, write to the   */
/* Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.    */
/*                                                                            */
/* Files needed for use:                                                      */
/*     * hi64.c            ---- Driver source                                 */
/*     * hkernel.c         ---- Kernel source                                 */
/*     * hi64.h            ---- General include file                          */
/*     * typedefs.h        ---- Include file for DSIZE and ISIZE              */
/*     * README.md         ---- Benchmark documentation and usage information */
/******************************************************************************/

#include       <stdio.h>
#include       <stdlib.h>
#include       <stdint.h>
#include       <math.h>
#include       <time.h>

/******************************************************************************/
/*   Vendor Specific defines and includes                                     */
/******************************************************************************/

#if defined(NCUBE2S)       /* Correction for 25 MHz clock on Ncube 2s         */
#define NTIME 0.8
#else
#define NTIME 1.0
#endif

#if defined(MP1) || defined(MP2)
#include       <mpl.h>     /* MasPar specific includes                        */
#include       <mp_time.h> /* MasPar timer prototypes and data declarations   */
#else
#include       <sys/time.h>/* Unix timer prototypes and data declarations     */
#endif

#if defined(SGI)           /* SGI specific includes for shared memory support.*/
#include       <ulocks.h>  
#include       <task.h>
#endif

#if defined(KSR)           /* Kendall Square Research includes for shared mem */
#include       <ksr/numarg.h> 
#endif

#if defined(SUNMOS) || defined(PARAGON)
#include       <nx.h>      /* Intel Paragon specific includes                 */
#define USE_DCLOCK         /* Use the Intel timer routine.                    */
#endif

#if defined(SP2)
#include       <mpproto.h>
#endif

#if defined(MPI)
#include <mpi.h>
#include <unistd.h>
#endif

#if defined(T3D)
#include <mpp/shmem.h>
#endif

#ifdef FORTRAN             /* SGI Fortran compiler forces trailing underscore */
#define Hint hint_
#endif


/******************************************************************************/
/*      Macro Declarations                                                    */
/******************************************************************************/
#define MIN(x,y) (((x) > (y))? (y): (x))
#define MAX(x,y) (((x) > (y))? (x): (y))

/******************************************************************************/
/*      Adjustable Defines                                                    */
/*      See README.md for more detailed information.                          */
/******************************************************************************/

#ifndef ADVANCE
#define ADVANCE    1.2589  /* Multiplier. We use roughly 1 decibel step size. */
                           /* Closer to 1.0 takes longer to run, but might    */
                           /* produce slightly higher net QUIPS.              */
#endif

#define NCHUNK     4       /* Number of chunks for scatter decomposition      */
                           /* Larger numbers increase time to first result    */
                           /* (latency) but sample domain more evenly.        */
#ifndef NSAMP
#define NSAMP      5000    /* Maximum number of QUIPS measurements            */
                           /* Increase if needed, e.g. if ADVANCE is smaller  */
#endif

#ifndef NTRIAL
#define NTRIAL     5       /* Normal number of times to run a trial           */
                           /* Increase if computer is prone to interruption   */
#endif

#ifndef PATIENCE
#define PATIENCE   10      /* Number of times to rerun a bogus trial          */
#endif

#ifndef RUNTM
#define RUNTM      0.5     /* Target time, seconds. Reduce for high-res timer.*/
                           /* Should be much larger than timer resolution.    */
#endif

#ifndef STOPRT
#define STOPRT     0.15    /* Ratio of current to peak QUIPS to stop at       */
                           /* Smaller numbers will beat on virtual memory.    */
#endif

#ifndef STOPTM
#define STOPTM     60      /* Longest time acceptable, seconds.  Most systems */
                           /* run out of decent-speed memory well before this */
#endif

#define MXPROC     32      /* Maximum number of processors to use in shared   */
                           /* memory configuration. Adjust as necessary.      */

/******************************************************************************/
/*      Non-Adjustable Defines                                                */
/******************************************************************************/
#define DSREFS     90
#define ISREFS     10
#define HI         0       /* Index of high values                            */
#define LO         1       /* Index of low values                             */
#define TRUE       (1==1)  /* Self explanatory                                */
#define FALSE      (1==0)  /* Self explanatory                                */
#define MSIZE      int64_t /* Integral type for allocating memory             */
#define NMIN       2       /* Minimum number of subintervals                  */
#define ANSWER     0.38629436111989061883446424291635136151000268720510508241360
#define MAXMESSAGE 512

/******************************************************************************/
/*      Type Defines                                                          */
/******************************************************************************/
#include "typedefs.h"      /* Sets data type DSIZE and index type ISIZE       */

/******************************************************************************/
/*      Type Declarations                                                     */
/******************************************************************************/
typedef struct {
    DSIZE   ahi,           /* Upper bound on rectangle areas                  */
            alo,           /* Lower bound on rectangle areas                  */
            dx,            /* Interval widths                                 */
            flh,           /* Function values of left coordinates, high       */
            fll,           /* Function values of left coordinates, low        */
            frh,           /* Function values of right coordinates, high      */
            frl,           /* Function values of right coordinates, low       */
            xl,            /* Left x-coordinates of subintervals              */
            xr;            /* Right x-coordinates of subintervals             */
    }RECT;

typedef struct {
    double  t,             /* Time for a given run                            */
            qp,            /* QUIPS for a given run                           */
			delq;          /* Change in Quality                               */
    ISIZE   n;             /* Subintervals for a given run                    */
	int     laps;
    }Speed;

typedef ISIZE ERROR;


/******************************************************************************/
/*      Prototypes                                                            */
/******************************************************************************/
#pragma mips_frequency_hint FREQUENT When
double  When();

double
Run(int laps,
    DSIZE *gamut, DSIZE scx, DSIZE scy, DSIZE dmax, ISIZE memry, ERROR *eflag);

#if defined(MP1) || defined(MP2)
DSIZE
Hint(DSIZE *scx, DSIZE *scy, DSIZE *xmax, ISIZE *mcnt, plural RECT *rect,
                          plural DSIZE *errs, plural ISIZE *ixes, ERROR *eflag);

#pragma mips_frequency_hint FREQUENT Hint
#elif defined(SGI) || defined(KSR) || defined(CONVEX)
void
Hint(DSIZE scx, DSIZE scy, ISIZE mcnt, int iproc, double gs[2],
               RECT *rect, DSIZE *errs, ISIZE *ixes, ERROR *eflag);

#pragma mips_frequency_hint FREQUENT Hint
#else
DSIZE
Hint(DSIZE *scx, DSIZE *scy, DSIZE *dmax, ISIZE *mcnt, RECT *rect,
                           DSIZE *errs, ISIZE *ixes, ERROR *eflag);
#pragma mips_frequency_hint FREQUENT Hint
#endif


