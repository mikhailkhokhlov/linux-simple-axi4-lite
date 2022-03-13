CC=arm-linux-gnueabihf-gcc

CFLAGS=-g
LDFLAGS=-Wl,--dynamic-linker -Wl,/lib/ld-linux.so.3

all: simple_axi_lite_test

cled_test: simple_axi_lite_test.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -vf *.o
	rm -vf simple_axi_lite_test
