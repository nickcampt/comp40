# Makefile for arith (Comp 40 Assignment 4)
#
#
# This Makefile is more verbose than necessary.  In each assignment
# we will simplify the Makefile using more powerful syntax and implicit rules.
#
# Last updated: February 16, 2016


############## Variables ###############

CC = gcc # The compiler being used

# Updating include path to use Comp 40 .h files and CII interfaces
IFLAGS = -I/comp/40/include -I/usr/sup/cii40/include/cii

# Compile flags
# Set debugging information, allow the c99 standard,
# max out warnings, and use the updated include path
# CFLAGS = -g -std=c99 -Wall -Wextra -Werror -Wfatal-errors -pedantic $(IFLAGS)
#
# For this assignment, we have to change things a little.  We need
# to use the GNU 99 standard to get the right items in time.h for the
# the timing support to compile.
#
CFLAGS = -g -std=gnu99 -Wall -Wextra -Werror -Wfatal-errors -pedantic $(IFLAGS)

# Linking flags
# Set debugging information and update linking path
# to include course binaries and CII implementations
LDFLAGS = -g -L/comp/40/lib64 -L/usr/sup/cii40/lib64 -L/comp/40/lib64 -larith40

# Libraries needed for linking
# All programs cii40 (Hanson binaries) and *may* need -lm (math)
# 40locality is a catch-all for this assignment, netpbm is needed for pnm
# rt is for the "real time" timing library, which contains the clock support
LDLIBS = -l40locality -lbitpack -lum-dis -lcii40 -lcii -lm -lrt

# Collect all .h files in your directory.
# This way, you can never forget to add
# a local .h file in your dependencies.
#
# This bugs Mark, who dislikes false dependencies, but
# he agrees with Noah that you'll probably spend hours
# debugging if you forget to put .h files in your
# dependency list.
INCLUDES = $(shell echo *.h)

############### Rules ###############

all: um

## Compile step (.c files -> .o files)

# To get *any* .o file, compile its .c file with the following rule.
%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@

## Linking step (.o -> executable program)

test_map: test_map_segment.o segment.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

test_unmap: test_unmap_segment.o segment.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

test_load_from_memory: load_from_memory_test.o segment.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

test_conditional_move: test_conditional_move.o segment.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

test_load_program: test_load_program.o segment.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

test_store_in_memory: test_store_in_memory.o segment.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

test_segment: test_segment.o segment.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

um: um.o segment.o calculation.o IO.o execution.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

clean:
	rm -f test_conditional_move test_load_from_memory test_load_program test_map test_segment test_store_in_memory test_unmap *.o
