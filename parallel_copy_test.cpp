#include "parallel_copy.h"
#include <algorithm>
#include <iostream>
#include <iterator>

#define TABSIZE 16

int main()
{
	int* tab1 = new int[TABSIZE];
	int* tab2 = new int[TABSIZE];
	int* tab3 = new int[TABSIZE << 1];

	srand(time(NULL));
	std::generate(tab1, tab1 + TABSIZE, rand);
	parallel::copy(tab1 , tab1 + TABSIZE, tab2);
	std::merge(tab1 , tab1 + TABSIZE, tab2 , tab2 + TABSIZE, tab3);
	std::sort(tab3, tab3 + (TABSIZE << 1));
	std::copy(tab3, tab3 + (TABSIZE << 1), std::ostream_iterator<int>(std::cout,"\n"));
	
	delete[] tab1;
	delete[] tab2;
	delete[] tab3;
}
