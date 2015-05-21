TOPDIR=..
include $(TOPDIR)/make.inc


SMALLPACK = ieeeck.o ilaenv.o lsame.o scopy.o sisnan.o slae2.o slaebz.o \
            slaev2.o slaisnan.o slamch.o slaneg.o slanst.o slar1v.o slarnv.o \
            slarra.o slarrb.o slarrc.o slarrd.o slarre.o slarrf.o slarrj.o \
            slarrk.o slarrr.o slarrv.o slaruv.o slaset.o slasq2.o slasq3.o \
            slasq5.o slasq6.o slasrt.o slassq.o sscal.o sstegr.o sstemr.o \
            sswap.o xerbla.o


all: ../$(SMALLLAPACKLIB)

ALLOBJ=$(SMALLPACK)

../$(SMALLLAPACKLIB): $(ALLOBJ)
	$(ARCH) $(ARCHFLAGS) ../$(SMALLLAPACKLIB) $(ALLOBJ)
	$(RANLIB) ../$(SMALLLAPACKLIB)


$(SMALLLAPACKLIB): $(FRC)

FRC:
	@FRC=$(FRC)

clean:
	rm -f *.o

.c.o: 
	$(CC) $(CFLAGS) -c $<

#slaruv.o: slaruv.c ; $(CC) $(NOOPT)  -c $< -o $@
#dlaruv.o: dlaruv.c ; $(CC) $(NOOPT)  -c $< -o $@
#sla_wwaddw.o: sla_wwaddw.c ; $(CC) $(NOOPT)  -c $< -o $@
#dla_wwaddw.o: dla_wwaddw.c ; $(CC) $(NOOPT)  -c $< -o $@
#cla_wwaddw.o: cla_wwaddw.c ; $(CC) $(NOOPT)  -c $< -o $@
#zla_wwaddw.o: zla_wwaddw.c ; $(CC) $(NOOPT)  -c $< -o $@

