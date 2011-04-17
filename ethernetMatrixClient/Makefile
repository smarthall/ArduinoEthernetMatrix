UNAME := $(shell uname -s)
CC := gcc
CFLAGS := -Wall -pedantic -std=c99 -g
CPPFLAGS := -Wall -pedantic -g
LDFLAGS := -lstdc++

ifeq ($(UNAME),Darwin)
	CFLAGS := $(CFLAGS) -I/opt/local/include -L/opt/local/lib
endif

all: sendrandom sendinitial sendpnm sendletter sendblank sendorient sendstring slideanim countdisplays

viewport.o: viewport.c viewport.h
	$(CC) $(CFLAGS) -c viewport.c -o viewport.o

sendrandom: sendrandom.c
	$(CC) $(CFLAGS) sendrandom.c -o sendrandom

sendinitial: sendinitial.c
	$(CC) $(CFLAGS) sendinitial.c -o sendinitial

sendblank: sendblank.c
	$(CC) $(CFLAGS) sendblank.c -o sendblank

sendorient: sendorient.c
	$(CC) $(CFLAGS) sendorient.c -o sendorient

sendpnm: sendpnm.c
	$(CC) $(CFLAGS) -lnetpbm sendpnm.c -o sendpnm

sendletter: sendletter.c viewport.o
	$(CC) $(CFLAGS) -lnetpbm sendletter.c viewport.o -o sendletter

sendstring: sendstring.c viewport.o
	$(CC) $(CFLAGS) -lnetpbm sendstring.c viewport.o -o sendstring

slideanim: slideanim.c viewport.o
	$(CC) $(CFLAGS) -lnetpbm slideanim.c viewport.o -o slideanim


# C++ Programs
ethernetdisplay.o: ethernetdisplay.cpp ethernetdisplay.h
	$(CC) $(CPPFLAGS) -c ethernetdisplay.cpp -o ethernetdisplay.o

countdisplays.o: countdisplays.cpp
	$(CC) $(CPPFLAGS) -c countdisplays.cpp -o countdisplays.o

countdisplays: countdisplays.o ethernetdisplay.o
	$(CC) $(LDFLAGS) countdisplays.o ethernetdisplay.o -o countdisplays

clean:
	- rm -f sendrandom sendinitial sendpnm sendletter sendblank sendorient sendstring slideanim countdisplays
	- rm -f *.o
ifeq ($(UNAME),Darwin)
	- rm -rf *.dSYM
endif
