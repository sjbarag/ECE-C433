#### Info
Author: Sean Barag
Date: 16 February 2012
Course: ECEC433 (Winter 2012)

#### Contents
The following files are included in this .zip archive:
* p1a.c     Part 1a source file
* p1b.c     Part 1b source file
* p1c.h     Header file for p1c and p2a
* p1c.c     Part 1c source file, which gets data for p1c, p1d, and p1e.
            This is was done to reuse as much code as possible.
* p1d.c     Symbolic link to p1c.c
* p2a.c     Part 2a source file, which gets data for p2a, p2b, and p2c
* peter.c   A copy of the code written in class

* procData.py    Data processing script for p1d and p1e, to produce
                 gnuplot-compatible data
* ethDist.py     Data processing script for p2b and p2c
* data/plots.gp  gnuplot script to generate plots for p1d and p1e

* data/by\_millisecond.png    gnuplot output for packet arrival times, by millisecond
* data/by\_second.png         gnuplot output for packet arrival times, by second
* data/by\_minute.png         gnuplot output for packet arrival times, by minute
* data/cdf.png                gnuplot cumulative density function for packet sizes
* data/eth\_differences.txt   Description of the ethernet type distribution and
                              promiscuity differences

* data/timing\_data.txt      Captured data used for the plots
* data/eth\_data.txt         Captured data used for the ethernet type analysis

* README.md         This file
* Makefile          A GNU Make file
* instructions.pdf  A copy of the instructions
* combined.pdf      A print-ready PDF of all the code
* LICENSE           Software license for this project

#### Compiling
This project can be compiled using the included Makefile, assuming that GNU
`make` is installed on your system and that `gcc` is in your `PATH` environment
variable.

Should GNU `make` not work for you, just open the Makefile and use the targets
as written.

#### Running
Each application compiles into a binary in the `out/` directory.  As such, they
can be executed as follows:

    sudo out/p1a
    sudo out/p1b
    sudo out/p1c [TIME]
    sudo out/p2a [TIME]

The time can be omitted for programs that require one.  The default time is
only 5 seconds though.  If you forget to use sudo, the programs will each yell
at you and exit :).
