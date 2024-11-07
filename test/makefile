
export GCC=gcc
export CFLAGS+=-I$(PACKBITS_INCLUDE) -L$(PACKBITS_LIB) -g

.PHONY: all
all: bin/test
	gdb bin/test

.PHONY: test-bin
test-bin: scripts/test-packbits.bash scripts/test.bash
	env PATH="$(PATH):$(PACKBITS_BIN)" ./scripts/test-packbits.bash

.PHONY: clean
clean:
	rm -f src/test-packbits.o
	rm -f src/test-packbits-encoder.o
	rm -f src/test-packbits-decoder.o
	rm -f src/main.o

src/test-packbits.o: src/test-packbits.h src/test-packbits.c src/test.h $(PACKBITS_INCLUDE)
	$(GCC) $(CFLAGS) -c -o src/test-packbits.o src/test-packbits.c

src/test-packbits-encoder.o: src/test-packbits-encoder.h src/test-packbits-encoder.c src/test.h $(PACKBITS_INCLUDE)
	$(GCC) $(CFLAGS) -c -o src/test-packbits-encoder.o src/test-packbits-encoder.c

src/test-packbits-decoder.o: src/test-packbits-decoder.h src/test-packbits-decoder.c src/test.h $(PACKBITS_INCLUDE)
	$(GCC) $(CFLAGS) -c -o src/test-packbits-decoder.o src/test-packbits-decoder.c

src/main.o: src/main.c
	$(GCC) $(CFLAGS) -c -o src/main.o src/main.c

bin:
	mkdir -p bin

bin/test: src/main.o src/test-packbits.o src/test-packbits-encoder.o src/test-packbits-decoder.o $(PACKBITS_LIB) | bin
	$(GCC) $(CFLAGS) -o bin/test src/main.o src/test-packbits.o src/test-packbits-encoder.o src/test-packbits-decoder.o -lpackbits
