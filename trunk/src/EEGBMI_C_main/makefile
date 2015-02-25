#
# EEG-BMI prototype, makefile
#
# Frédéric Simard, 2014

# compile with gcc
CC        = g++ 
#-O3 -fomit-frame-pointer -finline-functions -foptimize-register-move -ftree-loop-ivcanon -freorder-blocks-and-partition

# need flags for math and real-time
LDFLAGS   = -lm -lrt
# name of the program
PROGRAM   = EEGBMI.exe
# files of the program
SRCS      = main.cpp dataset.cpp feature_scaler.cpp
OBJS      = $(SRCS:.c=.o)
INC      = -I ./EEGBMI_C_fda/eigen-lib -I ./EEGBMI_C_svm -I ./EEGBMI_C_fft -I ./EEGBMI_C_fda
LIB  	 = -L./EEGBMI_C_fft -lfft -L./EEGBMI_C_fda -lfda -L./EEGBMI_C_svm -lsvm

#
# Launch gcc
#

default: all

all: $(PROGRAM) 

$(PROGRAM): $(OBJS)
	$(CC) $(OBJS) $(INC) -o $(PROGRAM) $(LDFLAGS) $(LIB) 

clean:
	/bin/rm -f $(OBJS) $(PROGRAM)
