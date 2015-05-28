#
# EEG-BMI prototype, makefile
#
# Frédéric Simard, 2014

# compile with gcc
CC        = gcc 
               		 
# need flags for math and real-time
CFLAGS = 
LDFLAGS   = -L. -lm -lrt -lstegr -lf2c
# name of the program
PROGRAM   = lin_alg_test.exe
# files of the program
SRCS      = linear_algebra.c lin_alg_testbench.c
OBJS      = $(SRCS:.c=.o)

#
# Launch gcc
#

default: dependencies all subclean lin_alg.so

dependencies: 
	cd f2clib; make
	cd stegr; make

all: $(PROGRAM) 

$(PROGRAM): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(PROGRAM) $(LDFLAGS) 

lin_alg.so:
	$(CC) $(CFLAGS) -fPIC -c linear_algebra.c $(LDFLAGS) 
	$(CC) -shared linear_algebra.o $(CFLAGS) -o lin_alg.so $(LDFLAGS) 

subclean:
	/bin/rm -f $(OBJS)

clean:
	/bin/rm -f $(OBJS) $(PROGRAM)

#$ LD_LIBRARY_PATH=.
#$ export LD_LIBRARY_PATH
