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
/******************************************************************************/
/*      Defines                                                               */
/******************************************************************************/
#define NOERROR    0      /* Self explanatory                                 */
#define DISCRET    1      /* Error condition for out of discretization error  */
#define NOMEM      2      /* Error for out of memory                          */
/******************************************************************************/
/*      Type Defines                                                          */
/******************************************************************************/

#define QMSIZE long long

#if defined(LDOUBLE)
#define DSIZE long double

#elif defined(DOUBLE)
#define DSIZE double
#define DSIZEF REAL*8
#define MPI_DSIZE MPI_DOUBLE

#elif defined(FLOAT)
#define DSIZE float
#define DSIZEF REAL*4
#define MPI_DSIZE MPI_FLOAT

#elif defined(LONGLONG)
#define DSIZE long long
#define DSIZEF INTEGER*8

#elif defined(LONG)
#define DSIZE long
#define DSIZEF INTEGER*4

#elif defined(INT)
#define DSIZE int
#define DSIZEF INTEGER
#define MPI_DSIZE MPI_INT

#elif defined(SHORT)
#define DSIZE short
#define DSIZEF INTEGER*2

#elif defined(CHAR)
#define DSIZE char
#define DSIZEF INTEGER*1
#endif



#if defined(ILONGLONG)
#define ISIZE long long
#define ISIZEF INTEGER*8

#elif defined(ILONG)
#define ISIZE long
#define ISIZEF INTEGER*4

#elif defined(IINT)
#define ISIZE int
#define ISIZEF INTEGER
#endif
