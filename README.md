# The HI64 System Benchmark

The HI64 System Benchmark (Hierarchical Integration 64-Bit) is a benchmarking
tool derived from [HINT][1], originally developed at the Ames Laboratory at the
U.S. Department of Energy. It has been modified to take full advantage of modern
64-bit computer systems. This benchmark is designed as a holistic assessment of
system performance, including the CPU, caches, main memory, and storage.

## How it works

HI64, like its predecessor, assesses system performance by computing the upper
and lower bounds of the function `(1-x)/(1+x)` within the interval [0, 1] by
dividing the region into many subintervals, using only the fact that the
function is always decreasing. The program stores the data for each subinterval
in an array called `rect`. The error of this computation is determined by
summing for each interval the difference between the upper and lower bounds
multiplied by the width of the interval. The reciprocal of the error represents
the accuracy of the computation, referred to as **quality**.

The benchmark consists of a series of trials, each involving a number of runs of
the above computation (called *laps*) using a certain number of subintervals.
Each subsequent trial uses more subintervals, increasing the precision of the
result along with the amount of CPU time and memory required. For each trial,
the program keeps track of the amount of time and memory required to complete
the computation, as well as the quality of the result and other information.
Based on this information, a value called **quality improvements per second**
(**QUIPS**) is computed, representing the computer's performance for that trial.
Data collected on the QUIPS attained by a computer over time as the benchmark
progresses can be used for in-depth analysis of the performance characteristics
of the system (see **Interpreting output** below).

The benchmark runs continuously until a substantial drop in performance occurs.
This typically occurs when the computer runs out of main memory and resorts to
disk paging to complete the computation, resulting in a dramatic slowdown and an
attendant drop in QUIPS. Specifically, the benchmark stops when the QUIPS for
the previous trial drops belows a predefined ratio of the peak QUIPS attained
during the benchmark or when the shortest lap in a trial takes longer than a
predefined amount of time. Alternatively, the maximum amount of memory to use
can be set and the benchmark will stop when this limit is reached. (More details
on how to set these limits follow.)

## Building and configuration

HI64 can be built with most C compilers as long as 64-bit integers are
supported. Optimization levels higher than `-O1` have been found (at least on
GCC) to generate non-working code, so they should not be used. In this document,
we will assume a recent version of GCC.

Settings for HI64 are defined at build time, through a combination of compiler
options and macros defined in `hint.h`. The following macros specify data types
to be used internally and are set through the compiler command line. Primitive
data types, as well as types provided by the C header file <stdint.h> (such as
`uint32_t` and `int64_t`), can be used. These macros include:

 - `DSIZE`: The data type to use for the underlying computations. Both
   floating-point and integral types may be used, as long as they are signed
   (e.g `long` is a valid data type for DSIZE, while `unsigned long` is not).
   Larger types such as `double` and `int64_t` permit higher-quality results
   allowing for longer computation but may lower maximum QUIPS. Smaller data
   types such as `float` and `int32_t` tend to be faster but their lower
   precision limits the maximum quality of the result, resulting in a sharp drop
   in QUIPS as the precision of the variables is exhausted. (Due to a limitation
   in the timing code, 32-bit and smaller data types may not work.)
 - `ISIZE`: The data type to use for the index of the `rect` array. An integral
   data type is required. On most systems, `ISIZE` may be signed or unsigned;
   an unsigned type is preferred where possible as array indexes are always
   positive. At least one half the bits of effective precision of `DSIZE` is
   needed to attain the full precision available from `DSIZE`. For example, a
   `DSIZE` of `double` has 53 bits of effective precision (the length of the
   mantissa), so an `ISIZE` of `uint32_t` is sufficiently long to utilize this
   precision.

An example build command would look like this:

    gcc -O1 -funroll-loops -DDSIZE=double -DISIZE=int32_t -o hi64-double hi64.c hkernel.c

A Makefile is included to simplify compilation of the benchmark. Running `make`
or `mingw32-make` will generate binaries which use `DSIZE` of 16-, 32-, and
64-bit integers as well as single-, double-, and extended-precision
floating-point numbers. The Makefile currently assumes GCC; modify it as needed
if you are using a different compiler.

The macros in the "Adjustable Defines" section of `hi64.h` determine the
behavior of the benchmark, such as when to stop the benchmark and how many laps
to run per trial. Notable options include:

 - `ADVANCE`: The amount by which to multiply the number of subintervals for
   successive trial. This value must be greater than 1. Smaller values increase
   performance data sampling precision (see **Interpreting output** below) and
   may increase net QUIPS, but can make the benchmark take a very long time
   to complete. The default is `1.2589`.
 - `NCHUNK`: This value appears to be used in a machine-specific parallel
   version of the original code which is untested. This value may be ignored as
   it does not appear to be used in the serial version. The default is `4`.
 - `NSAMP`: The maximum number of performance data samples which may be stored.
   HI64 stores performance data for each trial in a fixed-size array, and lower
   values of `ADVANCE` result in more samples taken. Because memory is generally
   not an issue on modern systems, this value has been significantly increased
   from the value set in the original code. A value that is too low, such as 50,
   could cause HI64 to crash due to a buffer overflow as there may not be enough
   space to store all performance data. The default is `5000`.
 - `NTRIAL`: The minimum number of laps per trial. Higher numbers reduce
   QUIPS variability in systems with frequent interrupts but increase the amount
   time needed to complete the benchmark. High interrupt rates are common in
   interactive systems, including most consumer computers, but specialized
   systems which receive little user input and have few background tasks are
   less subject to frequent interrupts. The default is `5`.
 - `PATIENCE`: The number of times to rerun a particular trial if that trial
   took longer than the next trial. At the end of the main benchmark, HI64
   checks the performance data and reruns any trials where the following trial
   took less time to complete, starting from the last trial and working
   backwards. If, after `PATIENCE` runs of a particular trial, it still takes
   longer than the following trial, it is treated as having taken as long as the
   following trial. Increasing the `PATIENCE` value may increase net QUIPS
   especially on systems with frequent interrupts but may also cause the
   benchmark to take longer to finish. The default is `10`.
 - `RUNTM`: The minimum time to run each trial, in seconds. HI64 will use this
   value to determine how many laps to run in earlier trials so that there are
   enough laps to fill `RUNTM` seconds, but in any case no less than `NTRIAL`
   laps. Values that are too low may reduce the accuracy of the data collected
   during the early stages of the benchmark. The default is `0.5`.
 - `STOPRT`: The ratio of current QUIPS to the peak QUIPS below which the
   benchmark will be stopped. Lowering this value will cause the benchmark to
   take much longer to complete as it will be less likely to detect that the
   system has run out of memory. The default is `0.15`, or 15% of the maximum
   QUIPS attained during the benchmark.
 - `STOPTM`: The maximum permitted time, in seconds, for a trial. The benchmark
   is stopped when a trial exceeds this time. This value should be set high
   enough to provide for the increasing amount of time needed to complete each
   trial as main memory fills up, but not so high that it exceeds the amount of
   time spent per lap when paging to disk. The default is `60`.

The remaining options may be ignored. The non-adjustable defines should not be
changed as doing so may break the benchmark.

These options may be set at the compiler command line by adding a parameter such
as `-DNTRIAL=3`, or as parameters to `make`, as in `NTRIAL=3`. Parameters which
may be set this way include `ADVANCE`, `NCHUNK`, `NTRIAL`, `PATIENCE`, `RUNTM`,
`STOPRT`, and `STOPTM`.

## Usage

The first parameter is used to set a memory limit in megabytes. The benchmark
will end when this memory limit is reached. If this value is zero, then no limit
is set. If no parameters are given, or if this parameter is invalid, no limit
will be set either.

The second parameter determines the name of the output directory, prefixed by `data`. For example, if the parameter is `foo`, it will attempt to write output
to the directory called `datafoo`. The name of the output file used is the same
as the name by which the executable was invoked. If there are fewer than two
parameters, it will attempt to write to the directory named `data`. If the
directory does not exist, the benchmark will not run. (This unusual output
behavior was inherited from the original HINT code and will be changed in a
future release to make the program easier to use.)

The program runs until the `STOPRT` or `STOPTM` thresholds are reached, or until
the specified memory limit is reached. If no memory limit is specified, this
generally means that the system will run out of memory and start swapping to
disk heavily before the benchmark is finished.

## Interpreting output

HI64 writes performance data collected during the benchmark to file at the
end of the benchmark (see Usage above). Each row of the output file represents
one trial, with the data sorted in descending order of time. Six columns of
output are generated, as follows:

 1. The time for the fastest lap in the trial.
 2. The QUIPS for that trial.
 3. The quality of the result obtained for that trial.
 4. The number of subintervals used.
 5. The amount of memory used.
 6. The measured memory bandwidth, in MB/s.

These data may be used with applications such as Microsoft Excel,
OpenOffice/LibreOffice Calc, or gnuplot to generate graphs which provide a
visualization of the performance characteristics of a computer. Information
about a computer's caches and memory size can be obtained through interpretation
of these graphs. In addition, comparing output for runs involving different data
types, such as `double` and `int64_t`, can provide insight about the system's
integer and floating-point compute performance. For more information about how
to read these graphs, see the article ["Understanding HINT Graphs"][2] at the
HINT website. (HI64 is based on HINT and produces functionally equivalent
output.)

HI64 also generates a value called **net QUIPS**, a representation of the
overall performance of the system. This value is calculated based on the total
times for the shortest lap in each trials and the total QUIPS and is printed to
standard output at the end of the benchmark.

## More information

More information about the benchmark can be obtained in the README file located
in the "old-files" directory. This file was supplied with the original HINT
distribution, so some parts may be out of date, but it still contains useful
information.


  [1]: http://hint.byu.edu/
  [2]: http://hint.byu.edu/tutorials/graphs/index.html