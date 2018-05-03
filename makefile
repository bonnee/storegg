#.DEFAULT_GOAL := info
.PHONY: build clean default

#default: info;

all: in out in_handle

in: 
	cd ./bin && gcc -o in ../src/in.c 

out: 
	cd ./bin && gcc -o out ../src/out.c 

in_handle:
	cd ./bin && gcc -o in_handle ../src/in_handle.c

build: creadirectory all

clean:
	@rm -rf bin

creadirectory:
	@mkdir -p ./bin



