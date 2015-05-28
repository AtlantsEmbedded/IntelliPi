TOPDIR=..
include $(TOPDIR)/make.inc

# listing of the objects to be included
ALLOBJ = ieeeck.o ilaenv.o lsame.o scopy.o sisnan.o slae2.o slaebz.o \
        slaev2.o slaisnan.o slamch.o slaneg.o slanst.o slar1v.o slarnv.o \
        slarra.o slarrb.o slarrc.o slarrd.o slarre.o slarrf.o slarrj.o \
        slarrk.o slarrr.o slarrv.o slaruv.o slaset.o slasq2.o slasq3.o \
        slasq5.o slasq6.o slasrt.o slassq.o sscal.o sstegr.o sstemr.o \
        sswap.o iparmq.o slasq4.o xerbla.o

# call the sequence of etiquettes to get everything done
all: ../$(STEGRLIB) libstegr.so smallapack_install

#this line actually call the %.o : %.c line below
../$(STEGRLIB): $(ALLOBJ)

# this will build every object file, from the .c
%.o: %.c 
	$(CC) $(CFLAGS) -c $<

# this builds the shared library
libstegr.so: $(OFILES)
	cc -shared -o libstegr.so $(ALLOBJ)

# this copies it to the folder below
smallapack_install: 
	mv libstegr.so ..

clean:
	rm -f *.o




