#
# MLP parallel solution, makefile
#
# Frédéric Simard, 2014

# compile with gcc
CC        = gcc -O2 -march=core2

# need flags for POSIX thread, math and real-time
LDFLAGS   = -lpthread -lm -lrt
# name of the program
PROGRAM   = MLP_C.exe
# files of the program
SRCS      = main.c Network.c Wrapper.c ticket_barrier.c SpinCheckpoint.c
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
