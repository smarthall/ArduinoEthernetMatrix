UNAME := $(shell uname -s)
CC := g++
CPPFLAGS := -Wall -pedantic -g
LDFLAGS := -lstdc++ -lnetpbm

ifeq ($(UNAME),Darwin)
	CPPFLAGS := $(CPPFLAGS) -I/opt/local/include
	LDFLAGS := $(LDFLAGS) -L/opt/local/lib
endif

all: sendrandom sendinitial sendpnm sendblank sendorient sendstring slideanim countdisplays displayinfo

# Compile C++ API
ethernetdisplay.o: ethernetdisplay.cpp ethernetdisplay.h
	$(CC) $(CPPFLAGS) -c ethernetdisplay.cpp -o ethernetdisplay.o

# Compile C++ Programs
countdisplays.o: countdisplays.cpp
	$(CC) $(CPPFLAGS) -c countdisplays.cpp -o countdisplays.o

displayinfo.o: displayinfo.cpp
	$(CC) $(CPPFLAGS) -c displayinfo.cpp -o displayinfo.o

slideanim.o: slideanim.cpp
	$(CC) $(CPPFLAGS) -c slideanim.cpp -o slideanim.o

sendblank.o: sendblank.cpp
	$(CC) $(CPPFLAGS) -c sendblank.cpp -o sendblank.o

sendinitial.o: sendinitial.cpp
	$(CC) $(CPPFLAGS) -c sendinitial.cpp -o sendinitial.o

sendorient.o: sendorient.cpp
	$(CC) $(CPPFLAGS) -c sendorient.cpp -o sendorient.o

sendrandom.o: sendrandom.cpp
	$(CC) $(CPPFLAGS) -c sendrandom.cpp -o sendrandom.o

sendpnm.o: sendpnm.cpp
	$(CC) $(CPPFLAGS) -c sendpnm.cpp -o sendpnm.o

sendstring.o: sendstring.cpp
	$(CC) $(CPPFLAGS) -c sendstring.cpp -o sendstring.o


# Link C++ Programs
countdisplays: countdisplays.o ethernetdisplay.o
	$(CC) $(LDFLAGS) countdisplays.o ethernetdisplay.o -o countdisplays

displayinfo: displayinfo.o ethernetdisplay.o
	$(CC) $(LDFLAGS) displayinfo.o ethernetdisplay.o -o displayinfo

slideanim: slideanim.o ethernetdisplay.o
	$(CC) $(LDFLAGS) slideanim.o ethernetdisplay.o -o slideanim

sendblank: sendblank.o ethernetdisplay.o
	$(CC) $(LDFLAGS) sendblank.o ethernetdisplay.o -o sendblank

sendinitial: sendinitial.o ethernetdisplay.o
	$(CC) $(LDFLAGS) sendinitial.o ethernetdisplay.o -o sendinitial

sendorient: sendorient.o ethernetdisplay.o
	$(CC) $(LDFLAGS) sendorient.o ethernetdisplay.o -o sendorient

sendrandom: sendrandom.o ethernetdisplay.o
	$(CC) $(LDFLAGS) sendrandom.o ethernetdisplay.o -o sendrandom

sendpnm: sendpnm.o ethernetdisplay.o
	$(CC) $(LDFLAGS) sendpnm.o ethernetdisplay.o -o sendpnm

sendstring: sendstring.o ethernetdisplay.o
	$(CC) $(LDFLAGS) sendstring.o ethernetdisplay.o -o sendstring

clean:
	- rm -f sendrandom sendinitial sendpnm sendblank sendorient sendstring slideanim countdisplays displayinfo
	- rm -f *.o
ifeq ($(UNAME),Darwin)
	- rm -rf *.dSYM
endif
