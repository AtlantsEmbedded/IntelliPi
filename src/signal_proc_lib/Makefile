#############################################################################
# Makefile for building: data_interface 
#############################################################################

MAKEFILE      = Makefile

####### Compiler, tools and options

CC            = gcc
CXX           = $(CXX)
CFLAGS        = -pipe -O2 -Wall -W   $(DEFINES) $(X86_DEFINES) $(RASPI_DEFINES)
CXXFLAGS      =  -pipe -O2 -Wall -W $(DEFINES) $(X86_DEFINES) $(RASPI_DEFINES)
LINK          = $(CC)
LFLAGS        = 

ifeq ($(ARCH), arm)
	RASPI_DEFINES  =-DRASPI=1
	INCPATH       = -I. \
                -Iinclude \
                -I$(STAGING_DIR)/include
	CFLAGS=$(TARGET_CFLAGS) -pipe -O2 -Wall -W  $(DEFINES) $(X86_DEFINES) $(RASPI_DEFINES)
else ifeq ($(ARCH), x86)
	ARCH_LIBS 	  =
	X86_DEFINES   =-DX86=1 -g
	INCPATH       = -I. \
               		-Iinclude
else 
	ARCH_LIBS 	  =
	X86_DEFINES   =-DX86=1 -g
	INCPATH       = -I. \
               		-Iinclude
endif

LIBS          =-L$(STAGING_DIR)/lib -L$(STAGING_DIR)/usr/lib -lm $(ARCH_LIBS)
AR            = ar cqs
RANLIB        = 
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = cp -f
COPY_DIR      = cp -f -R
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = src/signal_proc_testbench.c \
				src/pink_noise.c \
				src/fft.c \
				src/simple_parametric_signals.c
OBJECTS       = src/signal_proc_testbench.o \
				src/pink_noise.o \
				src/fft.o \
				src/simple_parametric_signals.o
DIST          = 
DESTDIR       = #avoid trailing-slash linebreak
TARGET        = signal_proc_testbench


first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH)  -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: start compile

start:
	@echo "\nStarting Make---------------------------------------\n"
	@echo " >> $(ARCH) selected....\n"
	 
compile: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)
	@echo "\nLinking----------------------------------------------\n"
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

dist:


####### Compile

fake_eeg_example.o: src/signal_proc.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o signal_proc.o src/signal_proc.c

pink_noise.o: src/pink_noise.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o pink_noise.o src/pink_noise.c
	
fft.o: src/fft.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o fft.o src/fft.c
	
simple_parametric_signals.o: src/simple_parametric_signals.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o simple_parametric_signals.o src/simple_parametric_signals.c
	
####### Install

install:   FORCE

uninstall:   FORCE

clean:
	find . -name "*.o" -type f -delete
	rm $(TARGET)

FORCE:
