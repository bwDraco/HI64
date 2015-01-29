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

## 0.3.0 (December 28, 2014)

 - Added the ability to limit memory use. The memory limit is specified at
   runtime in megabytes as the first parameter to the benchmark executable. The
   output directory name is now specified by the second parameter.
 - Fixed a typo in the out-of-memory error message.
 
## 0.3.1 (December 28, 2014)

 - Updated documentation to reflect the memory limiting feature.
 - Increased the maximum file name length to 200 characters.
 - Minor stylistic changes in code.

## 0.3.2 (December 28, 2014)

 - Fixed a bug in the memory limiting code which caused execution to continue
   two trials past the limit.
 - Fixed a bug affecting the filename string buffer limiting the filename to 80
   characters rather than the intended 160.

## 0.3.3 (January 29, 2015)

 - Fixed a bug in the Makefile that resulted in binaries not being generated for
   single-precision floating-point computations.
