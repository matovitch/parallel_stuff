#ifndef __PARALLEL_COPY_H__
#define __PARALLEL_COPY_H__

#include <thread>
#include <algorithm>

namespace parallel
{

template <class InputIterator, class OutputIterator>
OutputIterator copy (InputIterator first, InputIterator last, OutputIterator result,
					int cores = std::thread::hardware_concurrency())
{
	if (cores < 1) cores = 1;
	std::thread* pool = new std::thread[cores];

	for (int i = 0; i < cores; i++)
		pool[i] = std::thread(std::copy<InputIterator, OutputIterator>, 
			first + i * (last - first) / cores, 
			first + (i + 1) * (last - first) / cores, 
			result + i * (last - first) / cores);

	for (int i = 0; i < cores; i++)
		pool[i].join();

	delete[] pool;

	return result;
}

}

#endif