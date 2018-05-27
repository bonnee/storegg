#ifndef LOGIC_H
#define LOGIC_H
#include <stdio.h>
#include <stdlib.h>

int my_pow(int x, int y);

void num_to_bin(int num, int n_bit, int ar_delay, int *array);

void calc_output(int *input, int *output, int max_egg, int max_out);

int Log2n(int n);

double get_binary_digits(double n);
#endif