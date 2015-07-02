#
# EEG-BMI prototype, makefile
#
# Frédéric Simard, 2014

# compile with gcc
CC        = gcc 
               		 
# need flags for math and real-time
CFLAGS = 
LDFLAGS   = -L. -lm 
# name of the program
PROGRAM   = stats_test.exe
# files of the program
SRCS      = stats.c stats_testbench.c
OBJS      = $(SRCS:.c=.o)

#
# Launch gcc
#

default: dependencies all subclean stats.so

dependencies: 


all: $(PROGRAM) 

$(PROGRAM): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(PROGRAM) $(LDFLAGS) 

stats.so:
	$(CC) $(CFLAGS) -fPIC -c stats.c $(LDFLAGS) 
	$(CC) -shared stats.o $(CFLAGS) -o stats.so $(LDFLAGS) 

subclean:
	/bin/rm -f $(OBJS)

clean:
	/bin/rm -f $(OBJS) $(PROGRAM)

#$ LD_LIBRARY_PATH=.
#$ export LD_LIBRARY_PATH
