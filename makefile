.DEFAULT_GOAL := build
.PHONY: build clean default

default: build;

all: in out in_handle out_handle handler main

main: 
	cd ./bin && gcc -o main ../src/main.c

in: 
	cd ./bin && gcc -o in ../src/in.c ../src/io.c ../src/msg_queue.c

out: 
	cd ./bin && gcc -o out ../src/out.c ../src/io.c ../src/msg_queue.c

in_handle:
	cd ./bin && gcc -o in_handle ../src/in_handle.c ../src/msg_queue.c

out_handle:
	cd ./bin && gcc -o out_handle ../src/out_handle.c ../src/msg_queue.c

handler:
	cd ./bin && gcc -o handler ../src/handler.c ../src/msg_queue.c

build: creadirectory all

clean:
	@rm -rf bin

creadirectory:
	@mkdir -p ./bin



