
export GCC=gcc
export CFLAGS=-Wall
# export CFLAGS=-Wall -g -Og #for debug build.
# export CFLAGS=-Wall -O3 -finline-functions #for release build.

.PHONY: all
all: dist/include/packbits/packbits-encoder.h dist/include/packbits/packbits-decoder.h dist/include/packbits/packbits-realloc-func.h dist/include/packbits/packbits.h dist/include/packbits/packbits-errno.h dist/lib/libpackbits.a dist/lib/libpackbits.so bin/packbits

.PHONY: clean
clean:
	rm -f src/packbits-encoder.o
	rm -f src/packbits-decoder.o
	rm -f src/packbits-realloc-func.o
	rm -f src/packbits.o
	rm -f src/packbits-errno.o
	make -C test clean

.PHONY: test
test: all
	make -C test PACKBITS_INCLUDE=$(CURDIR)/dist/include PACKBITS_LIB=$(CURDIR)/dist/lib

.PHONY: test-bin
test-bin: all
	make -C test test-bin PACKBITS_BIN=$(CURDIR)/bin

src/packbits-encoder.o: src/packbits-encoder.c src/packbits-encoder.h src/packbits-errno.h
	$(GCC) $(CFLAGS) -c -o src/packbits-encoder.o src/packbits-encoder.c

src/packbits-decoder.o: src/packbits-decoder.c src/packbits-decoder.h src/packbits-errno.h
	$(GCC) $(CFLAGS) -c -o src/packbits-decoder.o src/packbits-decoder.c

src/packbits-realloc-func.o: src/packbits-realloc-func.c src/packbits-realloc-func.h
	$(GCC) $(CFLAGS) -c -o src/packbits-realloc-func.o src/packbits-realloc-func.c

src/packbits.o: src/packbits.h src/packbits.c src/packbits-realloc-func.h src/packbits-errno.h src/buffer.h
	$(GCC) $(CFLAGS) -c -o src/packbits.o src/packbits.c

src/packbits-errno.o: src/packbits-errno.h src/packbits-errno.c
	$(GCC) $(CFLAGS) -c -o src/packbits-errno.o src/packbits-errno.c

src/main.o: src/main.c src/main.c src/packbits-errno.h src/packbits-encoder.h src/packbits-decoder.h
	$(GCC) $(CFLAGS) -c -o src/main.o src/main.c

dist/include/packbits:
	mkdir -p dist/include/packbits

dist/lib:
	mkdir -p dist/lib

dist/include/packbits/packbits-encoder.h: src/packbits-encoder.h | dist/include/packbits
	cp src/packbits-encoder.h dist/include/packbits/packbits-encoder.h

dist/include/packbits/packbits-decoder.h: src/packbits-decoder.h | dist/include/packbits
	cp src/packbits-decoder.h dist/include/packbits/packbits-decoder.h

dist/include/packbits/packbits-realloc-func.h: src/packbits-realloc-func.h | dist/include/packbits
	cp src/packbits-realloc-func.h dist/include/packbits/packbits-realloc-func.h

dist/include/packbits/packbits.h: src/packbits.h | dist/include/packbits
	cp src/packbits.h dist/include/packbits/packbits.h

dist/include/packbits/packbits-errno.h: src/packbits-errno.h | dist/include/packbits
	cp src/packbits-errno.h dist/include/packbits/packbits-errno.h

dist/lib/libpackbits.a: src/packbits-encoder.o src/packbits-decoder.o src/packbits-realloc-func.o src/packbits.o src/packbits-errno.o | dist/lib
	ar r dist/lib/libpackbits.a src/packbits-encoder.o src/packbits-decoder.o src/packbits-realloc-func.o src/packbits.o src/packbits-errno.o

dist/lib/libpackbits.so: src/packbits-encoder.o src/packbits-decoder.o src/packbits-realloc-func.o src/packbits.o src/packbits-errno.o | dist/lib
	$(GCC) $(CFLAGS) -shared -o dist/lib/libpackbits.so src/packbits-encoder.o src/packbits-decoder.o src/packbits-realloc-func.o src/packbits.o src/packbits-errno.o

bin:
	mkdir -p bin

bin/packbits: src/main.o dist/lib | bin
	$(GCC) $(CFLAGS) -Ldist/lib -o bin/packbits src/main.o -lpackbits
