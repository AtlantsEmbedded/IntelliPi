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
CXX           := $(CC)
LEX           := flex
YACC          := yacc
CFLAGS        := -Wall -fPIC $(CFLAGS)
CXXFLAGS      := -Wall -fPIC $(CXXFLAGS)
LEXFLAGS      := 
YACCFLAGS     := -d
INCPATH       := -I. -I/include/ -I/usr/include/
LINK          := $(CC) 
LFLAGS        := -shared -Wl,-soname,$(TARGET)
LIBS	      := -L/lib/ -L/usr/lib -lm -lrt
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
				
OBJECTS       = $(SRCS:.cpp=.o)

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

base_example.cpp.o: base_example.cpp 

oneclass_solver.cpp.o: oneclass_solver.cpp 

svm.cpp.o: svm.cpp 

svr_solver.cpp.o: svr_solver.cpp 

classifier.cpp.o: classifier.cpp 

param.cpp.o: param.cpp 

example.cpp.o: example.cpp 

q_matrix.cpp.o:	q_matrix.cpp 

svm_solver.cpp.o: svm_solver.cpp 

model.cpp.o: model.cpp 

qp_solver.cpp.o: qp_solver.cpp

####### Install

install:  
	$(MKDIR) $(DESTDIR)/lib/
	$(COPY_FILE) -a lib$(LIBBASENAME).so* $(DESTDIR)/lib
	$(COPY_FILE) -a *.h $(DESTDIR)/usr/include/

uninstall:   FORCE

FORCE:
