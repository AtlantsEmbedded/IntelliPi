#
# EEG-BMI prototype, makefile
#
# Frédéric Simard, 2014

# compile with gcc
CC        = gcc 

# need flags for math and real-time
LDFLAGS   = -lm -lrt
# name of the program
PROGRAM   = lin_alg_test.exe
# files of the program
SRCS      = linear_algebra.c lin_alg_testbench.c
OBJS      = $(SRCS:.c=.o)

#
# Launch gcc
#

default: all

all: $(PROGRAM) 

$(PROGRAM): $(OBJS)
	$(CC) $(OBJS) -o $(PROGRAM) $(LDFLAGS) 

clean:
	/bin/rm -f $(OBJS) $(PROGRAM)
