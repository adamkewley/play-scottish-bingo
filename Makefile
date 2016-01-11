CC=clang
CCFLAGS=

obj/playing-cards.o: playing-cards.c playing-cards.h
	mkdir -p obj
	$(CC) $(CCFLAGS) -c playing-cards.c -o $@

dist/play-scottish-bingo: obj/playing-cards.o playing-cards.h play-scottish-bingo.c
	mkdir -p dist
	$(CC) $(CCFLAGS) $(filter %.c, $^) obj/playing-cards.o -o $@

dist/calculate-derangement-distribution: obj/playing-cards.o playing-cards.h calculate-derangement-distribution.c
	mkdir -p dist
	$(CC) $(CCFLAGS) $(filter %.c, $^) obj/playing-cards.o -o $@

.PHONY: all
all: dist/play-scottish-bingo dist/calculate-derangement-distribution

.PHONY: debug
debug: CCFLAGS += -g
debug: all

.PHONY: clean
clean:
	rm -r obj
	rm -r dist

.PHONE: rebuild
rebuild: clean all
