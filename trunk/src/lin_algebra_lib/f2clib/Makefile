#############################################################################
# Library Makefile for building: f2clib
# Frédéric Simard, 2014
#############################################################################
LIBBASENAME = f2clib
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
LD	      := $(CC)
CC            := $(CC)
CXX           := $(CXX)
LEX           := flex
YACC          := yacc
CFLAGS        := -Wall -fPIC $(CFLAGS)
CXXFLAGS      := -Wall -fPIC $(CXXFLAGS)
LEXFLAGS      := 
YACCFLAGS     := -d
INCPATH       := -I$(STAGING_DIR)/include -I$(STAGING_DIR)/usr/include -I../includes/
#INCPATH       := -I. -I/include/ -I/usr/include/ -I../includes/
LINK          := $(CC) 
LFLAGS        := -shared -Wl,-soname,$(TARGET)
LIBS          :=-L$(STAGING_DIR)/lib -L$(STAGING_DIR)/usr/lib -lm -lrt
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
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files
MISC =	f77vers.o i77vers.o s_rnge.o abort_.o exit_.o getarg_.o iargc_.o\
		getenv_.o signal_.o s_stop.o s_paus.o system_.o cabs.o ctype.o\
		derf_.o derfc_.o erf_.o erfc_.o sig_die.o uninit.o
POW =	pow_ci.o pow_dd.o pow_di.o pow_hh.o pow_ii.o pow_ri.o pow_zi.o pow_zz.o
CX =	c_abs.o c_cos.o c_div.o c_exp.o c_log.o c_sin.o c_sqrt.o
DCX =	z_abs.o z_cos.o z_div.o z_exp.o z_log.o z_sin.o z_sqrt.o
REAL =	r_abs.o r_acos.o r_asin.o r_atan.o r_atn2.o r_cnjg.o r_cos.o\
		r_cosh.o r_dim.o r_exp.o r_imag.o r_int.o\
		r_lg10.o r_log.o r_mod.o r_nint.o r_sign.o\
		r_sin.o r_sinh.o r_sqrt.o r_tan.o r_tanh.o
DBL =	d_abs.o d_acos.o d_asin.o d_atan.o d_atn2.o\
		d_cnjg.o d_cos.o d_cosh.o d_dim.o d_exp.o\
		d_imag.o d_int.o d_lg10.o d_log.o d_mod.o\
		d_nint.o d_prod.o d_sign.o d_sin.o d_sinh.o\
		d_sqrt.o d_tan.o d_tanh.o
INT =	i_abs.o i_dim.o i_dnnt.o i_indx.o i_len.o i_len_trim.o i_mod.o i_nint.o i_sign.o\
		lbitbits.o lbitshft.o i_ceiling.o
HALF =	h_abs.o h_dim.o h_dnnt.o h_indx.o h_len.o h_mod.o h_nint.o h_sign.o
CMP =	l_ge.o l_gt.o l_le.o l_lt.o hl_ge.o hl_gt.o hl_le.o hl_lt.o
EFL =	ef1asc_.o ef1cmc_.o
CHAR =	f77_aloc.o s_cat.o s_cmp.o s_copy.o
I77 =	backspac.o close.o dfe.o dolio.o due.o endfile.o err.o\
		fmt.o fmtlib.o ftell_.o iio.o ilnw.o inquire.o lread.o lwrite.o\
		open.o rdfmt.o rewind.o rsfe.o rsli.o rsne.o sfe.o sue.o\
		typesize.o uio.o util.o wref.o wrtfmt.o wsfe.o wsle.o wsne.o xwsne.o
QINT =	pow_qq.o qbitbits.o qbitshft.o ftell64_.o
TIME =	dtime_.o etime_.o


OBJECTS       = $(MISC) $(POW) $(CX) $(DCX) $(REAL) $(DBL) $(INT) \
	$(HALF) $(CMP) $(EFL) $(CHAR) $(I77) $(TIME)

SOURCES       = $(ALLSRCS:.o=.c)

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
	$(CC) -c -DSkip_f2c_Undefs $(CFLAGS) $*.c
	ld -r -x -o $*.xxx $*.o
	mv $*.xxx $*.o
	
####### Build rules

all: Makefile f2c.h signal1.h sysdep1.h $(TARGET) 
#$(TARGETA)

$(TARGET):  $(UICDECLS) $(OBJECTS) $(SUBLIBS) $(OBJCOMP)  
	-$(DEL_FILE) $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS) $(OBJCOMP)
	-ln -s $(TARGET) $(TARGET0)
	-ln -s $(TARGET) $(TARGET1)
	-ln -s $(TARGET) $(TARGET2)

#staticlib: $(TARGETA)

#$(TARGETA):  $(OBJECTS) $(OBJCOMP) 
#	-$(DEL_FILE) $(TARGETA) 
#	$(AR) $(AR_ARGS) $(TARGETA) $(OBJECTS) $(OBJMOC)

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

f77vers.o: f77vers.c
	$(CC) -c f77vers.c

i77vers.o: i77vers.c
	$(CC) -c i77vers.c
	
hadd: f2c.h0 f2ch.add
	cat f2c.h0 f2ch.add >f2c.h

signal1.h: signal1.h0
	cp signal1.h0 signal1.h

sysdep1.h: sysdep1.h0
	cp sysdep1.h0 sysdep1.h
	
	
backspac.o:	fio.h
close.o:	fio.h
dfe.o:		fio.h
dfe.o:		fmt.h
due.o:		fio.h
endfile.o:	fio.h rawio.h
err.o:		fio.h rawio.h
fmt.o:		fio.h
fmt.o:		fmt.h
iio.o:		fio.h
iio.o:		fmt.h
ilnw.o:		fio.h
ilnw.o:		lio.h
inquire.o:	fio.h
lread.o:	fio.h
lread.o:	fmt.h
lread.o:	lio.h
lread.o:	fp.h
lwrite.o:	fio.h
lwrite.o:	fmt.h
lwrite.o:	lio.h
open.o:		fio.h rawio.h
rdfmt.o:	fio.h
rdfmt.o:	fmt.h
rdfmt.o:	fp.h
rewind.o:	fio.h
rsfe.o:		fio.h
rsfe.o:		fmt.h
rsli.o:		fio.h
rsli.o:		lio.h
rsne.o:		fio.h
rsne.o:		lio.h
sfe.o:		fio.h
signbit.o:	arith.h
sue.o:		fio.h
uio.o:		fio.h
uninit.o:	arith.h
util.o:		fio.h
wref.o:		fio.h
wref.o:		fmt.h
wref.o:		fp.h
wrtfmt.o:	fio.h
wrtfmt.o:	fmt.h
wsfe.o:		fio.h
wsfe.o:		fmt.h
wsle.o:		fio.h
wsle.o:		fmt.h
wsle.o:		lio.h
wsne.o:		fio.h
wsne.o:		lio.h
xwsne.o:	fio.h
xwsne.o:	lio.h
xwsne.o:	fmt.h

arith.h: arithchk.c
	$(CC) $(CFLAGS) -DNO_FPINIT arithchk.c -lm ||\
	 $(CC) -DNO_LONG_LONG $(CFLAGS) -DNO_FPINIT arithchk.c -lm
	./a.out >arith.h
	rm -f a.out arithchk.o
	
####### Install

install:  
	$(MKDIR) $(DESTDIR)/lib/
	$(COPY_FILE) -a lib$(LIBBASENAME).so* $(DESTDIR)/lib
	$(COPY_FILE) -a *.h $(DESTDIR)/usr/include/

uninstall:   FORCE

FORCE:
