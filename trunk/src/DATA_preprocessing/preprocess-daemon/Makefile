#############################################################################
# Makefile for building: data_processing 
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
				src/data_input.c \
				src/feature_output.c \
				src/app_signal.c \
				src/preprocess_core.c \
				src/xml.c \
				src/supported_data_input/fake_data_generator.c \
				src/supported_data_input/shm_rd_buf.c \
				src/supported_feature_output/shm_wrt_buf.c \
				src/supported_feature_output/csv_file.c
OBJECTS       = src/main.o \
				src/data_input.o \
				src/app_signal.o \
				src/feature_output.o \
				src/preprocess_core.o \
				src/xml.o \
				src/supported_data_input/fake_data_generator.o \
				src/supported_data_input/shm_rd_buf.o \
				src/supported_feature_output/shm_wrt_buf.o \
				src/supported_feature_output/csv_file.o
DIST          = 
DESTDIR       = #avoid trailing-slash linebreak
TARGET        = data_preprocessing


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
	
data_input.o: src/data_input.c
	$(CC) -c $(CFLAGS) $(INCPATH) -o data_input.o src/data_input.c 

app_signal.o: src/app_signal.c
	$(CC) -c $(CFLAGS) $(INCPATH) -o app_signal.o src/app_signal.c 
	
feature_output.o: src/feature_output.c
	$(CC) -c $(CFLAGS) $(INCPATH) -o feature_output.o src/feature_output.c 
	
preprocess_core.o: src/preprocess_core.c
	$(CC) -c $(CFLAGS) $(INCPATH) -o preprocess_core.o src/preprocess_core.c 
	
xml.o: src/xml.c
	$(CC) -c $(CFLAGS) $(INCPATH) -o xml.o src/xml.c 
	
shm_rd_buf.o: src/supported_data_input/shm_rd_buf.c
	$(CC) -c $(CFLAGS) $(INCPATH) -o shm_rd_buf.o src/supported_data_input/shm_rd_buf.c
	
fake_data_generator.o: src/supported_data_input/fake_data_generator.c
	$(CC) -c $(CFLAGS) $(INCPATH) -o fake_data_generator.o src/supported_data_input/fake_data_generator.c 
	
shm_wrt_buf.o: src/supported_feature_output/shm_wrt_buf.c
	$(CC) -c $(CFLAGS) $(INCPATH) -o fake_data_generator.o src/supported_feature_output/shm_wrt_buf.c 
	
csv_file.o: src/supported_feature_output/csv_file.c
	$(CC) -c $(CFLAGS) $(INCPATH) -o csv_file.o src/supported_feature_output/csv_file.c 
	
####### Install

install:   FORCE

uninstall:   FORCE

clean:
	find . -name "*.o" -type f -delete
	rm $(TARGET)

FORCE:
