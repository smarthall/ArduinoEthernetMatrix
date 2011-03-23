UNAME := $(shell uname -s)
CC := gcc
CFLAGS := -Wall -pedantic -std=c99 -g

ifeq ($(UNAME),Darwin)
	CFLAGS := $(CFLAGS) -I/opt/local/include -L/opt/local/lib
endif

all: sendrandom sendinitial sendpnm sendletter sendblank sendorient sendstring slideanim

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

sendletter: sendletter.c viewport.c viewport.h
	$(CC) $(CFLAGS) -lnetpbm sendletter.c viewport.c -o sendletter

sendstring: sendstring.c viewport.c viewport.h
	$(CC) $(CFLAGS) -lnetpbm sendstring.c viewport.c -o sendstring

slideanim: slideanim.c viewport.c viewport.h
	$(CC) $(CFLAGS) -lnetpbm slideanim.c viewport.c -o slideanim

clean:
	rm sendrandom sendinitial sendpnm sendletter sendblank sendorient sendstring slideanim

