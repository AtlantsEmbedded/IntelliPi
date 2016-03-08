#############################################################################
# Makefile for building: gpio_menu
#############################################################################

MAKEFILE      = Makefile

####### Compiler, tools and options

CC            = $(CC)
CXX           = $(CXX)
CFLAGS        = -pipe -O2 -Wall -W -D_REENTRANT -fPIE  $(DEFINES) $(X86_DEFINES) $(RASPI_DEFINES)
CXXFLAGS      =  -pipe -O2 -Wall -W -D_REENTRANT -fPIE $(DEFINES) $(X86_DEFINES) $(RASPI_DEFINES)
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
	CFLAGS=$(TARGET_CFLAGS) -pipe -O2 -Wall -W -D_REENTRANT -fPIE  $(DEFINES) $(X86_DEFINES) $(RASPI_DEFINES)
else ifeq ($(ARCH), x86)
	ARCH_LIBS 	  =
	X86_DEFINES   =-DX86=1
	INCPATH       = -I. \
               		 -Iinclude \
else 
	ARCH_LIBS 	  =
endif

LIBS          =-L$(STAGING_DIR)/lib -L$(STAGING_DIR)/usr/lib -lpthread -lezxml -lglib-2.0 $(ARCH_LIBS)
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
				src/xml.c \
				src/user_input.c \
				src/menu_structure.c \
				src/menu_item.c \
				src/event.c \
				src/socket.c \
				src/hardware.c
OBJECTS       = src/main.o \
				src/xml.o \
				src/user_input.o \
				src/menu_structure.o \
				src/menu_item.o  \
				src/event.o \
				src/socket.o \
				src/hardware.o
DIST          = src/main.c \
				src/xml.c \
				src/user_input.c \
				src/menu_structure.c \
				src/menu_item.c \
				src/event.c \
				src/socket.c \
				src/hardware.c
DESTDIR       = #avoid trailing-slash linebreak
TARGET        = gpio_interface


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

xml.o: src/xml.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o xml.o src/xml.c 

user_input.o: src/user_input.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o user_input.o src/user_input.c 
	
menu_structure.o: src/menu_structure.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o menu_structure.o src/menu_structure.c 
	
menu_item.o: src/menu_item.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o menu_item.o src/menu_item.c 
	
socket.o: src/socket.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o socket.o src/socket.c 
	
event.o: src/event.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o event.o src/event.c 
	
hardware.o: src/hardware.c
	$(CC) -c $(CFLAGS) $(INCPATH) -o hardware.o src/hardware.c 

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:
