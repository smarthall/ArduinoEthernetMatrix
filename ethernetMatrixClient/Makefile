CC := gcc
CFLAGS := -std=c99

all: sendrandom sendinitial

sendrandom: sendrandom.c
	$(CC) $(CFLAGS) sendrandom.c -o sendrandom

sendinitial: sendinitial.c
	$(CC) $(CFLAGS) sendinitial.c -o sendinitial

clean:
	rm sendrandom sendinitial

