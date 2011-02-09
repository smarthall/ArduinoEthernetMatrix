CC := gcc
CFLAGS := -std=c99

all: sendrandom sendinitial sendpnm

sendrandom: sendrandom.c
	$(CC) $(CFLAGS) sendrandom.c -o sendrandom

sendinitial: sendinitial.c
	$(CC) $(CFLAGS) sendinitial.c -o sendinitial

sendpnm: sendpnm.c
	$(CC) $(CFLAGS) -lnetpbm sendpnm.c -o sendpnm

clean:
	rm sendrandom sendinitial sendpnm

