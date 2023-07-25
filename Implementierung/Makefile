CFLAGS=-std=gnu11 -O2 -Wall -Wextra -Wpedantic -g

.PHONY: clean all


all: main
main: main.c padding_md2.c checksum_md2.c hash_md2.c file_reader.c test.c implementation_md2.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f main
