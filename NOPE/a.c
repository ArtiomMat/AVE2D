#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	int num;
	unsigned oc;
} numoc_t;

void MostOccurances(int *arr, unsigned size)
{
	int b = 0; // Biggest number
	unsigned oc = 0; // Occurances of the biggest number

	// Loop each time from a different starting position
	for (unsigned starter = 0; starter < size; starter++)
	{
		// Potential new biggest number infromation
		int newb = arr[starter];
		unsigned newoc = 0;
		// Attempt at optimization
		if (newb == b)
			continue;
		// Loop that counts the occurances of
		for (unsigned i = 0; i < size; i++)
		{
			if ()
		}
	}
}

int main()
{
	return 0;
}
