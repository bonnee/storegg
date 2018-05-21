CC = gcc
CFLAGS  = -Wall

.PHONY: build clean default mkdir

default: build

build: mkdir all

mkdir:
	@mkdir -p bin/

all: bin/main bin/handler bin/in_handle bin/out_handle bin/in bin/out

bin/%.o: src/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

bin/main: bin/main.o
	$(CC) $(CFLAGS) -o $@ $^

bin/handler: bin/handler.o bin/msg_queue.o bin/logic.o
	$(CC) $(CFLAGS) -o $@ $^

bin/in_handle: bin/in_handle.o bin/msg_queue.o
	$(CC) $(CFLAGS) -o $@ $^

bin/out_handle: bin/out_handle.o bin/msg_queue.o
	$(CC) $(CFLAGS) -o $@ $^

bin/in: bin/in.o bin/io.o bin/msg_queue.o
	$(CC) $(CFLAGS) -o $@ $^

bin/out: bin/out.o bin/io.o bin/msg_queue.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	@rm -rf bin/