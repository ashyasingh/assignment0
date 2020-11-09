
CC=gcc
AR=ar
CFLAGS=-I.
LFLAGS=-L.

%.o: %.c
	$(CC) -fPIC -c -o $@ $(CFLAGS) $<

LOBJS=objsect.o objsym.o objcopy.o

libobjdata.a: $(LOBJS)
	$(AR) rc $@ $(LOBJS)

getsections: getsections.o libobjdata.a
	$(CC) -o $@ $< $(LFLAGS) -lobjdata -lbfd

getsyms: getsyms.o libobjdata.a
	$(CC) -o $@ $< $(LFLAGS) -lobjdata -lbfd

gettxt: gettxt.o libobjdata.a
	$(CC) -o $@ $< $(LFLAGS) -lobjdata -lbfd

libobjdata.so: $(LOBJS)
	$(CC) -shared -o $@ $(LOBJS) -lbfd

getsections_dl: getsections_dl.o libobjdata.so
	$(CC) -o $@ $< $(LFLAGS) -lobjdata -ldl

getsyms_dl: getsyms_dl.o libobjdata.so
	$(CC) -o $@ $< $(LFLAGS) -lobjdata -ldl

gettxt_dl: gettxt_dl.o libobjdata.so
	$(CC) -o $@ $< $(LFLAGS) -lobjdata -ldl

BINS=getsections getsyms gettxt getsections_dl getsyms_dl gettxt_dl

all: $(BINS)

clean:
	rm -f *.o *.a *.so $(BINS) text-output*

zip:
	tar cf cs410_0.tar *.c Makefile stats README *.sh

