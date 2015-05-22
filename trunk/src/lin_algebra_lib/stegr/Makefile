TOPDIR=..
include $(TOPDIR)/make.inc


STEGR = ieeeck.o ilaenv.o lsame.o scopy.o sisnan.o slae2.o slaebz.o \
            slaev2.o slaisnan.o slamch.o slaneg.o slanst.o slar1v.o slarnv.o \
            slarra.o slarrb.o slarrc.o slarrd.o slarre.o slarrf.o slarrj.o \
            slarrk.o slarrr.o slarrv.o slaruv.o slaset.o slasq2.o slasq3.o \
            slasq5.o slasq6.o slasrt.o slassq.o sscal.o sstegr.o sstemr.o \
            sswap.o iparmq.o slasq4.o xerbla.o


all: ../$(STEGRLIB)

ALLOBJ=$(STEGR)

../$(STEGRLIB): $(ALLOBJ)
	$(ARCH) $(ARCHFLAGS) ../$(STEGRLIB) $(ALLOBJ)
	$(RANLIB) ../$(STEGRLIB)


$(STEGRLIB): $(FRC)

FRC:
	@FRC=$(FRC)

clean:
	rm -f *.o

.c.o: 
	$(CC) $(CFLAGS) -c $<


