#include "parallel_sort.h"
#include <algorithm>
#include <iostream>
#include <iterator>

#define TABSIZE 10

int main()
{
	int* tab = new int[TABSIZE];

	srand(time(NULL));
	std::generate(tab, tab + TABSIZE, rand);
	parallel::sort(tab , tab + TABSIZE);
	std::copy(tab, tab + TABSIZE, std::ostream_iterator<int>(std::cout,"\n"));
	
	delete[] tab;
}