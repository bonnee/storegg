#include "logic.h"

//custom basic pow function for int parameters
int my_pow(int x, int y)
{
	int res = 1;
	for (int i = 0; i < y; i++)
		res *= x;
	return res;
}

//function that convert decimal num to binary writing into an array with a specified delay from first pos
void num_to_bin(int num, int n_bit, int ar_delay, int *array)
{
	for (int i = n_bit - 1; i >= 0; i--)
	{
		int num_pow = my_pow(2, i);
		if (num >= num_pow)
		{
			array[(n_bit - 1 - i) + ar_delay] = 1;
			num -= num_pow;
		}
		else
		{
			array[(n_bit - 1 - i) + ar_delay] = 0;
		}
	}
}

void calc_output(int *input, int *output)
{

	//initialization
	for (int i = 0; i < 8; i++)
	{
		output[i] = 0;
	}

	int num_eggs = 0;
	//count eggs
	for (int i = 0; i < 6; i++)
	{
		num_eggs += input[i];
	}

	int mag_min = 0;
	int start = 6;
	//count min eggs in storage
	for (int i = start; i < start + 2; i++)
	{
		mag_min += my_pow(2, 1 - (i - start)) * input[i];
	}

	// [bit 0-2]
	//number of eggs in binary
	num_to_bin(num_eggs, 3, 0, output);

	// [bit 3-4]
	//check to move eggs from storage
	if (num_eggs < 6 && mag_min > 0)
	{
		int eggs_to_move = 6 - num_eggs;
		if (eggs_to_move > mag_min) //not enough in storage
			eggs_to_move = mag_min;
		num_to_bin(eggs_to_move, 2, 3, output);
	}

	// [bit 5-7]
	//check to order eggs to storage
	if (mag_min < 3 && num_eggs + mag_min < 6)
	{		
		int order_eggs;					  	// know exactly num storage eggs and need some other
		if (6 - mag_min - num_eggs == 6)	// cartone e magazzino vuoto, rischio di chiederne 6
			order_eggs = 5 - mag_min;
		else
			order_eggs = 6 - mag_min - num_eggs;
		num_to_bin(order_eggs, 3, 5, output);
	}
}