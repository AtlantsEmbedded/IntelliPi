#############################################################################
# Library Makefile for building: EEG-BMI prototype
# Provided by Frédéric Simard, 2014
#############################################################################
LIBBASENAME = eeg_svm
MAJVER = 1
MINVER = 0
PATCHVER = 0
# Dont touch these TARGET definitions  they're needed below
TARGET        = lib$(LIBBASENAME).so.$(MAJVER).$(MINVER).$(PATCHVER)
TARGETA       = lib$(LIBBASENAME).a
TARGETD       = lib$(LIBBASENAME).so.$(MAJVER).$(MINVER).$(PATCHVER)
TARGET0       = lib$(LIBBASENAME).so
TARGET1       = lib$(LIBBASENAME).so.$(MAJVER)
TARGET2       = lib$(LIBBASENAME).so.$(MAJVER).$(MINVER)

####### Compiler, tools and options
LD	      	  := $(CC)
CC            := $(CC)
CXX           := $(CXX)
LEX           := flex
YACC          := yacc
CFLAGS        := -Wall -fPIC $(CFLAGS)
CXXFLAGS      := -Wall -fPIC $(CXXFLAGS)
LEXFLAGS      := 
YACCFLAGS     := -d
INCPATH       := -I$(STAGING_DIR)/include -I$(STAGING_DIR)/usr/include -I$(STAGING_DIR)/usr/include/uClibc++
#INCPATH       := -I. -I/include/ -I/usr/include/
LINK          := $(CC) 
LFLAGS        := -shared -Wl,-soname,$(TARGET)
LIBS          :=-L$(STAGING_DIR)/lib -L$(STAGING_DIR)/usr/lib -nodefaultlibs -luClibc++
#LIBS	      := -L/lib/ -L/usr/lib -lm -lrt
AR            := ar
AR_ARGS	      := cqs	
RANLIB        := 
TAR           = tar -cf
GZIP	      = gzip -9f
COPY          = cp -f
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
INSTALL_FILE  = $(COPY_FILE)
INSTALL_DIR   = $(COPY_DIR)
DEL_FILE      = rm -f
SYMLINK       = ln -sf
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS = test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = svm.cpp \
				base_example.cpp \
				oneclass_solver.cpp \
				classifier.cpp \
				example.cpp \
				svr_solver.cpp \
				param.cpp \
				q_matrix.cpp \
				model.cpp \
				svm_solver.cpp \
				qp_solver.cpp
				
OBJECTS       = svm.o \
				base_example.o \
				oneclass_solver.o \
				classifier.o \
				example.o \
				svr_solver.o \
				param.o \
				q_matrix.o \
				model.o \
				svm_solver.o \
				qp_solver.o

first: all
####### Implicit rules

.SUFFIXES: .c .o .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

####### Build rules

all: Makefile  $(TARGET) $(TARGETA)

$(TARGET):  $(UICDECLS) $(OBJECTS) $(SUBLIBS) $(OBJCOMP)  
	-$(DEL_FILE) $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS) $(OBJCOMP)
	-ln -s $(TARGET) $(TARGET0)
	-ln -s $(TARGET) $(TARGET1)
	-ln -s $(TARGET) $(TARGET2)

staticlib: $(TARGETA)

$(TARGETA):  $(OBJECTS) $(OBJCOMP) 
	-$(DEL_FILE) $(TARGETA) 
	$(AR) $(AR_ARGS) $(TARGETA) $(OBJECTS) $(OBJMOC)

yaccclean:
lexclean:
clean: 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core *.so*


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) $(TARGET0) $(TARGET1) $(TARGET2) $(TARGETA)

####### Compile

base_example.o: base_example.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o base_example.o base_example.cpp

oneclass_solver.o: oneclass_solver.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o oneclass_solver.o oneclass_solver.cpp 

svm.o: svm.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svm.o svm.cpp

svr_solver.o: svr_solver.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svr_solver.o svr_solver.cpp

classifier.o: classifier.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o classifier.o classifier.cpp 

param.o: param.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o param.o param.cpp 

example.o: example.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o example.o example.cpp

q_matrix.o: q_matrix.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o q_matrix.o q_matrix.cpp 

svm_solver.o: svm_solver.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o svm_solver.o svm_solver.cpp 

model.o: model.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o model.o model.cpp 

qp_solver.o: qp_solver.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o qp_solver.o qp_solver.cpp

####### Install

install:  
	$(MKDIR) $(DESTDIR)/lib/
	$(COPY_FILE) -a lib$(LIBBASENAME).so* $(DESTDIR)/lib
	$(COPY_FILE) -a *.h $(DESTDIR)/usr/include/

uninstall:   FORCE

FORCE:
