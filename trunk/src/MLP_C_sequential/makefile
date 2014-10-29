#
# MLP sequential, makefile
#

CC        = gcc -O3 -fomit-frame-pointer -finline-functions -foptimize-register-move -ftree-loop-ivcanon -freorder-blocks-and-partition

#

LDFLAGS   = -lm
PROGRAM   = MLP_C.exe
SRCS      = main.c Network.c Wrapper.c
OBJS      = $(SRCS:.c=.o)

#
# Targets
#

default: all

all: $(PROGRAM) 

$(PROGRAM): $(OBJS)
	$(CC) $(OBJS) -o $(PROGRAM) $(LDFLAGS)

clean:
	/bin/rm -f $(OBJS) $(PROGRAM)
