#############################################################################
# Makefile for building: MindBX_testbench 
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
	ARCH_LIBS = -lwiringPi -lwiringPiDev
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

LIBS          =-L$(STAGING_DIR)/lib -L$(STAGING_DIR)/usr/lib -lm -lpthread $(ARCH_LIBS)
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

SOURCES       = src/main.c \
				src/mindbx_lib.c 

OBJECTS       = src/main.o \
				src/mindbx_lib.o 

DIST          = 
DESTDIR       = #avoid trailing-slash linebreak
TARGET        = mindbx_testbench


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

main.o: src/main.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o main.o src/main.c

mindbx_lib.o: src/mindbx_lib.c
	$(CC) -c $(CFLAGS) $(INCPATH) -o mindbx_lib.o src/mindbx_lib.c
	
####### Install

install:   FORCE

uninstall:   FORCE

clean:
	find . -name "*.o" -type f -delete
	rm $(TARGET)

FORCE:
