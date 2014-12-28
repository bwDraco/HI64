# Change log for the HI64 System Benchmark

## 0.1.0 (October 25, 2014)

 - First release under the HI64 name.

## 0.2.0 (December 27, 2014)

 - Added Makefile for easier compilation.
 - Set default PATIENCE to 10. This is because modern computer systems tend to
   have frequent interrupts, so increasing this value increases benchmark
   accuracy.
 - Added support for setting key compile-time parameters using command-line
   arguments to the compiler or Make. Parameters which may be set this way
   include `ADVANCE`, `NCHUNK`, `NTRIAL`, `PATIENCE`, `RUNTM`, `STOPRT`, and
   `STOPTM`.