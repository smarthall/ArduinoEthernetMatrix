UNAME := $(shell uname -s)
CC := gcc
CFLAGS := -Wall -pedantic -std=c99 -g
CPPFLAGS := -Wall -pedantic -g
LDFLAGS := -lstdc++ -lnetpbm

ifeq ($(UNAME),Darwin)
	CFLAGS := $(CFLAGS) -I/opt/local/include -L/opt/local/lib
endif

all: sendrandom sendinitial sendpnm sendletter sendblank sendorient sendstring slideanim countdisplays displayinfo

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

# C++ Programs
ethernetdisplay.o: ethernetdisplay.cpp ethernetdisplay.h
	$(CC) $(CPPFLAGS) -c ethernetdisplay.cpp -o ethernetdisplay.o

countdisplays.o: countdisplays.cpp
	$(CC) $(CPPFLAGS) -c countdisplays.cpp -o countdisplays.o

displayinfo.o: displayinfo.cpp
	$(CC) $(CPPFLAGS) -c displayinfo.cpp -o displayinfo.o

slideanim.o: slideanim.cpp
	$(CC) $(CPPFLAGS) -c slideanim.cpp -o slideanim.o

countdisplays: countdisplays.o ethernetdisplay.o
	$(CC) $(LDFLAGS) countdisplays.o ethernetdisplay.o -o countdisplays

displayinfo: displayinfo.o ethernetdisplay.o
	$(CC) $(LDFLAGS) displayinfo.o ethernetdisplay.o -o displayinfo

slideanim: slideanim.o ethernetdisplay.o
	$(CC) $(LDFLAGS) slideanim.o ethernetdisplay.o -o slideanim

clean:
	- rm -f sendrandom sendinitial sendpnm sendletter sendblank sendorient sendstring slideanim countdisplays displayinfo
	- rm -f *.o
ifeq ($(UNAME),Darwin)
	- rm -rf *.dSYM
endif
