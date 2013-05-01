#ifndef __PARALLEL_SORT_H__
#define __PARALLEL_SORT_H__

#include <thread>
#include <algorithm>

template <class RandomAccessIter, class... Compare>
static void thread_manager(RandomAccessIter first, unsigned size,
						  RandomAccessIter store, unsigned depth,
						  std::thread* pool)
{
	if (depth == 1)
	{
		pool[0] = std::thread(std::sort<RandomAccessIter, Compare...>,
		 							store, store + size);
		return;
	}

	std::thread son0 = std::thread(thread_manager<RandomAccessIter, Compare...>,
										store, size >> 1,
										first, depth >> 1, pool);
	std::thread son1 = std::thread(thread_manager<RandomAccessIter, Compare...>,
										store + (size >> 1), size - (size >> 1),
										first + (size >> 1), depth >> 1,
										pool + (depth >> 1));

	son0.join();
	son1.join();

	for (unsigned i = 0; i < depth; ++i)
		pool[i].join();

	RandomAccessIter* begcuts = new RandomAccessIter[depth + 1];
	RandomAccessIter* endcuts = new RandomAccessIter[depth + 1];

	begcuts[0] = first;
	begcuts[depth] = first + (size >> 1);
	endcuts[0] = first + (size >> 1);
	endcuts[depth] = first + size;

	unsigned count = 0;

	for (int i = 1; i < depth; ++i) {
		begcuts[i] = first + i * (size/(2 * depth));

		endcuts[i] = std::lower_bound(endcuts[i-1],
								first + size,
								*(begcuts[i]));

		pool[i - 1] = std::thread(std::merge<RandomAccessIter, 
								RandomAccessIter, RandomAccessIter, Compare...>, 
								begcuts[i-1],
								begcuts[i],
								endcuts[i-1],
								endcuts[i],
								store + count);

		count += (endcuts[i] - endcuts[i-1]) + (begcuts[i] - begcuts[i-1]);
	}

	pool[depth - 1] = std::thread(std::merge<RandomAccessIter, 
								RandomAccessIter, RandomAccessIter, Compare...>, 
								begcuts[depth-1],
								begcuts[depth],
								endcuts[depth-1],
								endcuts[depth],
								store + count);

}

template <class RandomAccessIter, class... Compare>
void parallel_sort(RandomAccessIter first, RandomAccessIter last)
{
	typedef typename std::iterator_traits<RandomAccessIter>::value_type T;
	T* store = new T[last - first];

	unsigned cores = std::thread::hardware_concurrency();
	unsigned log2cores = std::log2(std::max(1, cores));
	cores = 1 << log2cores;

	std::thread* pool = new std::thread[cores];

	if (log2cores % 2)
	{
		std::copy(first, last, store);
		thread_manager<RandomAccessIter, Compare...>(first, last - first, store, cores, pool);
		for (int i = 0; i < cores; ++i)
			pool[i].join();
		std::copy(store, store + (last - first), first);
	}
	else
	{
		thread_manager<RandomAccessIter, Compare...>(store, last - first, first, cores, pool);
		for (int i = 0; i < cores; ++i)
			pool[i].join();
	}

	delete[] store; 
}

#endif