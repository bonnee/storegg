#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int pinExport(int pin);

int pinUnexport(int pin);

int pinDirection(int pin, int mode);

int pinRead(int pin);

int pinWrite(int pin, int value);

#endif