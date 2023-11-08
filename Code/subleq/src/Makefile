SRCS = boot.asq menu.asq \
       puts.asq gets.asq putint.asq getint.asq \
       bubblesort.asq calc.asq factorial.asq primes.asq \
       ascii.asq data.asq

CC = cc

all: asq sq hex

asq:
	cat $(SRCS) | ../thirdparty/sqasm > image.sq

sq:
	$(CC) sq.c -o sq

hex:
	python3 ../util/sq2hex.py < image.sq > image.hex


clean:
	rm image.sq sq
