#############################################################################
# Makefile for building: buzzer_bench
#############################################################################

MAKEFILE      = Makefile

####### Compiler, tools and options

CC            = gcc
CXX           = $(CXX)
CFLAGS        = -pipe -O2 -Wall -W   $(DEFINES) $(X86_DEFINES) $(RASPI_DEFINES)
CXXFLAGS      =  -pipe -O2 -Wall -W $(DEFINES) $(X86_DEFINES) $(RASPI_DEFINES)
LINK          = $(CC)
LFLAGS        = 
GLIB2_CC	  = `pkg-config --cflags glib-2.0`
GLIB2_LINK	  = `pkg-config --libs glib-2.0`

ifeq ($(ARCH), arm)
	ARCH_LIBS = -lwiringPi -lwiringPiDev
	RASPI_DEFINES  =-DRASPI=1
	INCPATH       = -I. \
                -Iinclude \
                -I$(STAGING_DIR)/include \
                -I$(STAGING_DIR)/usr/include/glib-2.0/
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

LIBS          =-L$(STAGING_DIR)/lib -L$(STAGING_DIR)/usr/lib -lm -lpthread -lezxml -lglib-2.0 $(ARCH_LIBS)
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
				src/app_signal.c \
				src/buzzer_lib.c \
				src/feature_input.c \
				src/feature_processing.c \
				src/ipc_status_comm.c \
				src/xml.c \
				src/supported_feature_input/fake_feature_generator.c \
				src/supported_feature_input/shm_rd_buf.c
OBJECTS       = src/main.o \
				src/app_signal.o \
				src/buzzer_lib.o \
				src/feature_input.o \
				src/feature_processing.o \
				src/ipc_status_comm.o \
				src/xml.o \
				src/supported_feature_input/fake_feature_generator.o \
				src/supported_feature_input/shm_rd_buf.o
DESTDIR       = #avoid trailing-slash linebreak
TARGET        = braintone_app


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
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS) $(GLIB2_LINK)

dist:


####### Compile

main.o: src/main.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o main.o src/main.c
	
app_signal.o: src/app_signal.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o app_signal.o src/app_signal.c
	
buzzer_lib.o: src/buzzer_lib.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o buzzer_lib.o src/buzzer_lib.c
	
feature_input.o: src/feature_input.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o feature_input.o src/feature_input.c
	
feature_processing.o: src/feature_processing.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o feature_processing.o src/feature_processing.c
	
ipc_status_comm.o: src/ipc_status_comm.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o ipc_status_comm.o src/ipc_status_comm.c
	
xml.o: src/xml.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o xml.o src/xml.c
	
fake_feature_generator.o: src/supported_feature_input/fake_feature_generator.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o fake_feature_generator.o src/supported_feature_input/fake_feature_generator.c
	
shm_rd_buf.o: src/supported_feature_input/shm_rd_buf.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o shm_rd_buf.o src/supported_feature_input/shm_rd_buf.c

####### Install

install:   FORCE

uninstall:   FORCE

clean:
	find . -name "*.o" -type f -delete
	rm $(TARGET)

FORCE:
