#############################################################################
# Makefile for building: gpio_menu
#############################################################################

MAKEFILE      = Makefile

####### Compiler, tools and options

CC            = $(CC)
CXX           = $(CXX)
DEFINES       = -D_DEBUG
CFLAGS        = -pipe -O2 -g -Wall -W -D_REENTRANT -fPIE $(DEFINES)
CXXFLAGS      = -pipe -O2 -g -Wall -W -D_REENTRANT -fPIE $(DEFINES) \
				-I$(STAGING_DIR)/usr/include/uClibc++
INCPATH       = -I. -Iinclude
LINK          = $(CC)
LFLAGS        = 

ifeq ($(ARCH), arm)
	ARCH_LIBS = 
	RASPI_DEFINES  =-DRASPI=1
	INCPATH       = -I. \
	-Iinclude \
	-I$(STAGING_DIR)/include \
	CFLAGS=$(TARGET_CFLAGS) -pipe -O2 -Wall -W -D_REENTRANT \
	-fPIE  $(DEFINES) $(X86_DEFINES) $(RASPI_DEFINES)
else ifeq ($(ARCH), x86)
	ARCH_LIBS 	  =
	X86_DEFINES   =-DX86=1
	INCPATH       = -I. \
	-Iinclude \
else 
	ARCH_LIBS 	  =
endif

LIBS=-L$(STAGING_DIR)/lib -L$(STAGING_DIR)/usr/lib -lpthread \
		-lm -lrt -leeg_fda -leeg_fft -leeg_svm \
		 -luClibc++ $(ARCH_LIBS)


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

SOURCES       = main.cpp \
				dataset.cpp \
				feature_scaler.cpp
OBJECTS       = main.o \
				dataset.o \
				feature_scaler.o
DIST          = main.cpp \
				dataset.cpp \
				feature_scaler.cpp
DESTDIR       = #avoid trailing-slash linebreak
TARGET        = EEGBMI.exe


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

all: clean start compile

start:
	@echo "\nStarting Make---------------------------------------\n"
	 
compile: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)
	@echo "\nLinking----------------------------------------------\n"
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

dist: 

clean: 
	@echo "\nClean-----------------------------------------------\n"
	-$(DEL_FILE) $(TARGET)
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core
	@echo "\nClean complete--------------------------------------\n"


####### Compile

main.o: main.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

dataset.o: dataset.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o dataset.o dataset.cpp

feature_scaler.o: feature_scaler.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o feature_scaler.o feature_scaler.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:
