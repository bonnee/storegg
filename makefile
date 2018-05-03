.DEFAULT_GOAL := info
.PHONY: build clean default

default: info;

all: 
	cd ./bin && gcc -o progetto ../src/progetto.c


build: creadirectory all

clean:
	@rm -rf bin

creadirectory:
	@mkdir -p ./bin

info: 
	cat file.txt


