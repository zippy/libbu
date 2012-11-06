# libbu++

Libbu++ is a general purpose, cross-platform utility library for C++.  It
covers a huge range of topics including compression and encryption, string
handling, containers, networking, and more.

Libbu++ is also under constant development, this means that some API may change
over time, but we try to make sure that everything stays as compatible as
possible.

## License

Libbu++ is distributed under a rather permissive BSD-like license, full details
are included in the file LICENSE included.

If these terms are insufficient or you desire alternative licensing, please
contact Xagasoft.

## Why libbu++?

I created libbu++ originally to try out some container classes like a hash
table, linked list, and some specialized trees for some game projects.  It's
evolved since then, but started being used more and more when we discovered that
it was faster and had a smaller memory footprint than alternative libraries on
some systems.

Some libraries are faster, and some libraries may have more features, but very
few of them are also as free as libbu++ is.  Hopefully, libbu++ will continue
to evolve and get faster, more stable, and fully featured.

A few things that are easy to do cross platform with libbu++:

 * Threading
 * Stream handling (files, sockets, memory buffers)
 * Stream filters (compression, encryption, base64, hex, and more)
 * Formatting values and complex objects as text
 * Serialization
 * Creating network servers and clients
 * Random number generation
 * Type safe signals and slots
 * Uuids (guids)
 * Managing settings in platform specific storage systems
 * Unit testing

Some specific, unique features:

 * Myriad, a block-allocated stream multiplexing file format
 * MyriadFS, a posix-compliant file system built on top of Myriad
 * TAF, the Textual Archive Format which makes config files and metadata easy
   to deal with.
 * MiniCron, a cron-like system for executing

## Requirements

Personally, I've only compiled using gcc (g++) 4 series, information on
success building with other compilers is greatly appreciated.

Libraries:

 * libc
 * libz
 * bzip2
 * lzma

The RegEx class is currently a wrapper around the glibc regex functions.
Development is underway to replace that with a custom regex system, but until
then the Regex class will not compile on any systems that do not have this
functionality (ahem, Windows).

If you want to compile without libz, bzip2, lzma, or regex support, simply
delete the respective files before compiling.  Each of these is only used in
one class internally (a .cpp and .h file).

For windows, a copy of libz and bzip2 all compiled and ready to go for mingw
are included (32bit).

### GNU/Linux

This is the platform I do all my work on, it should work great.

### Windows

I've only tried building libbu++ against MingW, the 32 bit version.  It works
quite well except for the regex functions not being available.

### Mac OSX

I don't own a mac, but I would love to support the Mac.  Unfortunately that's
difficult to do.  I have had successful builds on OSX, but Mac support may be
lagging behind by a decent amount.

## Compiling

The preferred method for building libbu++ is using Xagasoft build.  It also
includes a Makefile since build isn't really very standard yet.  The Makefile
simply tries it's best to compile as much of the library as possible.  It will
also setup a directory called *bu* in the libbu++ directory and create symlinks
to all library header files there to simulate an installed system.

Currently, only a static link library is built to make development easier.

To install you can use the ./checkinst.sh script included, this will setup
libbu++ using only symlinks so that you can update and develop while maintaining
a standard installation model.

I haven't written a real install target for build or make yet, the assumption
is that libbu++.a will be available in a standard location (e.g. /usr/lib) and
that the headers will be in a directory named bu in your system.

## Using libbu++

Either include the source code you want in your project, or build a library and
link against libbu++.  In either case, each class gets it's own header file,
you can include them like this:

    #include <bu/string.h>
    #include <bu/sio.h>
    #include <bu/file.h>
    #include <bu/bzip2.h>

When you compile, just make sure you link against the library, and anything
else you may need, i.e.

    g++ test.cpp -o test -lbu++ -lbz2

## File Layout

Within the src directory there are several sub directories:

 * *stable* - All code in this directory is well tested, nearly fully featured,
   and unlikely to change too much.
 * *unstable* - All code in this directory is pretty well tested, working
   towards becoming fully featured, but the API may change a little before it's
   moved to stable.
 * *experimental* - All code in this directory may or may not survive, and is
   currently being tested.  The API may change rapidly, or the code is being
   developed currently.
 * *tests* - Each .cpp file in this directory will be compiled into a
   stand-alone executable.  This is used primarily during development, the code
   in these tests is generally turned into a unit test eventually and the test
   file deleted.
 * *unit* - This directory is full of .unit files used to generate libbu++ unit
   tests.  We could use a lot more unit tests, but there are a few good and
   important ones so far. The libbu++ tool mkunit is used to turn .unit files
   into .cpp source code.
 * *tools* - Each source file in this directory becomes an executable, these
   are built by default.
 * *extra* - This directory includes source for programs that have extra
   dependencies, or only work on particular operating systems.  These are not
   built by default.
 * *doxy* - Doxygen is used for all documentation, this directory contains extra
   guide pages and documentation that doesn't belong anywhere else.
 * *compat* - Contains source files for specific platforms.


