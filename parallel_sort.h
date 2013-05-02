#ifndef __PARALLEL_SORT_H__
#define __PARALLEL_SORT_H__

#include <thread>
#include "parallel_merge.h"
#include "parallel_copy.h"

namespace parallel
{

template <class RandomAccessIter, class... Compare>
static void thread_manager(RandomAccessIter first, unsigned size,
						  RandomAccessIter store, unsigned cores)
{
	if (cores == 1)
	{
		std::thread leaf_sort(std::sort<RandomAccessIter, Compare...>,
		 							store, store + size);
		leaf_sort.join();
		return;
	}

	std::thread son0 = std::thread(thread_manager<RandomAccessIter, Compare...>,
										store, size >> 1,
										first, cores >> 1);
	std::thread son1 = std::thread(thread_manager<RandomAccessIter, Compare...>,
									store + (size >> 1), size - (size >> 1),
									first + (size >> 1), cores - (cores >> 1));

	son0.join();
	son1.join();

	parallel::merge(first, first + (size >> 1), first + (size >> 1),
					first + size, store, cores);

}

template <unsigned Cores = 0, class RandomAccessIter, class... Compare>
void sort(RandomAccessIter first, RandomAccessIter last, 
			int cores = std::thread::hardware_concurrency())
{
	typedef typename std::iterator_traits<RandomAccessIter>::value_type T;
	T* store = new T[last - first];

	if (cores < 1) cores = 1;

	if ((int)std::log2(cores) % 2)
	{
		parallel::copy(first, last, store, cores);
		thread_manager<RandomAccessIter, Compare...>(first, last - first, store, cores);
		parallel::copy(store, store + (last - first), first, cores);
	}
	else
	{
		thread_manager<RandomAccessIter, Compare...>(store, last - first, first, cores);
	}

	delete[] store; 
}

}

#endif