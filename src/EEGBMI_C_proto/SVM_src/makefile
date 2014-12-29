#
# EEG-BMI prototype, makefile
#
# Frédéric Simard, 2014

# compile with gcc
CC        = g++

# need flags for math and real-time
LDFLAGS   = -lm -lrt
# name of the program
PROGRAM   = svm_learn.exe
# files of the program
SRCS      = svm_learn.cpp base_example.cpp classifier.cpp example.cpp model.cpp param.cpp q_matrix.cpp qp_solver.cpp svm_solver.cpp  svr_solver.cpp oneclass_solver.cpp
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
