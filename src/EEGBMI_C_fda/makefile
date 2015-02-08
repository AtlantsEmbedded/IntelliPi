#
# EEG-BMI prototype, makefile
#
# Frédéric Simard, 2014

# compile with gcc
CC        = g++ 

# need flags for math and real-time
LDFLAGS   = -lm -lrt
# name of the program
PROGRAM   = fda_proto.exe
# files of the program
SRCS      = fda_proto_1.cpp
OBJS      = $(SRCS:.c=.o)

#
# Launch gcc
#

default: all

all: $(PROGRAM) 

$(PROGRAM): $(OBJS)
	$(CC) $(OBJS) -I eigen-lib -o $(PROGRAM) $(LDFLAGS) 

clean:
	/bin/rm -f $(OBJS) $(PROGRAM)
