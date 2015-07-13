#
# MLP parallel solution, makefile
#
# Frédéric Simard, 2014

# compile with gcc
CC        = gcc 

# need flags for POSIX thread, math and real-time
LDFLAGS   = -lpthread -lm -lrt
# name of the program
PROGRAM   = shm_server.a
# files of the program
SRCS      = main.c
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
