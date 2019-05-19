CC=gcc
CFLAGS=-I.
DEPS = common.h
LIBS=-lm

all: cli srv


cli: cli.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

srv: srv.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


.PHONY: clean

clean:
	rm -f *.o cli srv
