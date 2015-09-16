#
# EEG-BMI prototype, makefile
#
# Frédéric Simard, 2014

# compile with gcc
CC        = gcc 

# need flags for math and real-time
LDFLAGS   = -lm -lrt
# name of the program
PROGRAM   = EEGBMI_proto.exe
# files of the program
SRCS      = main.c wrapper.c filt_downsamp.c
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
