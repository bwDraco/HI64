# The HI64 System Benchmark

The HI64 System Benchmark (Hierarchical Integration 64-Bit) is a benchmarking tool derived from [HINT][1], originally developed at the Ames Laboratory at the U.S. Department of Energy. It has been modified to take full advantage of modern 64-bit computer systems. This benchmark is designed as a holistic assessment of system performance, including the CPU, caches, main memory, and storage.

## How it works

HI64, like its predecessor, assesses system performance by computing the upper and lower bounds of the function `(1/x)/(1+x)` within the interval [0, 1] by dividing the region into many subintervals, using only the fact that the function is always decreasing. The program stores the data for each subinterval in an array called `rect`. The error of this computation is determined by summing for each interval the difference between the upper and lower bounds multiplied by the width of the interval. The reciprocal of the error represents the precision of the computation, referred to as **quality**.

The benchmark consists of a series of trials, each of which consists of a number of runs of the above computation using a certain number of subintervals. Each subsequent trial uses more subintervals, increasing the precision of the result along with the amount of CPU time and memory required. For each trial, the program keeps track of the amount of time and memory required to complete the computation, as well as the quality of the result and other information. Based on this information, a value called **quality improvements per second** (**QUIPS**) is computed, representing the computer's performance for that trial. The QUIPS attained by a computer over time as the benchmark progresses can be used for in-depth assessment of the performance characteristics of the system.

The benchmark runs continuously until a substantial drop in performance occurs. This typically occurs when the computer runs out of main memory and resorts to disk paging to complete the computation, resulting in a dramatic slowdown and an attendant drop in QUIPS. Specifically, the benchmark stops when the QUIPS for the previous trial drops belows a predefined ratio of the peak QUIPS attained during the benchmark or when the slowest run in a trial takes longer than a predefined amount of time. (More details on how to set these limits follow.)

## Configuration

Settings for HINT are defined at build time, through a combination of compiler options and macros defined in `hint.h`. The following macros are set through the compiler command line:

 - `DSIZE`: The data type to use for the underlying computations. Both floating-point and integral types may be used, as long as they are signed. Larger types such as `double` and `__int64` permit higher-quality results allowing for longer computation but may lower maximum QUIPS. Smaller data types such as `float` and `__int32` tend to be faster but their lower precision limits the maximum quality of the result, resulting in a sharp drop in QUIPS as the precision of the variables is exhausted. (Due to a limitation in the timing code, 32-bit and smaller data types may not work.)
 - `ISIZE`: The data type to use for the index of the `rect` array. An integral data type with at least one half the bits of effective precision of `DSIZE` is needed to attain the full precision available from `DSIZE`. For example, a `DSIZE` of `double` has 53 bits of effective precision (the length of the mantissa), so `__int32` is sufficiently long to fully utilize this precision.

*TODO*: Add hi64.h options

## Interpreting output

*TODO*

## More information

More information about the benchmark can be obtained in the README file located in the "old-files" directory. This file was supplied with the original HINT distribution, so some parts may be out of date, but it still contains useful information.


  [1]: http://hint.byu.edu/ 