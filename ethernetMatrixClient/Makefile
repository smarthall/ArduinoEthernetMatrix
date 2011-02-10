CC := gcc
CFLAGS := -std=c99

all: sendrandom sendinitial sendpnm sendletter

sendrandom: sendrandom.c
	$(CC) $(CFLAGS) sendrandom.c -o sendrandom

sendinitial: sendinitial.c
	$(CC) $(CFLAGS) sendinitial.c -o sendinitial

sendpnm: sendpnm.c
	$(CC) $(CFLAGS) -lnetpbm sendpnm.c -o sendpnm


sendletter: sendletter.c
	$(CC) $(CFLAGS) -lnetpbm sendletter.c -o sendletter

clean:
	rm sendrandom sendinitial sendpnm sendletter

