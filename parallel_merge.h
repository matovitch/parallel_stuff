#ifndef __PARALLEL_MERGE_H__
#define __PARALLEL_MERGE_H__

#include <thread>
#include <algorithm>
#include <iostream>

namespace parallel
{

template <class InputIterator1, class InputIterator2, class OutputIterator, class... Compare>
 OutputIterator merge (InputIterator1 first1, InputIterator1 last1,
                        InputIterator2 first2, InputIterator2 last2,
                        OutputIterator result, Compare... comp, 
                        int cores = std::thread::hardware_concurrency())
{
	if (cores < 1) cores = 1;
	std::thread* pool = new std::thread[cores];

	InputIterator1 begins[cores];
	InputIterator2 ends[cores];

	int offset = 0;

	begins[0] = first1;
	ends[0] = first2;

	for (int i = 1; i < cores; i++)
	{
		begins[i] = first1 + i * (last1 - first1) / cores;
		ends[i] = std::lower_bound(ends[i-1], last2,
								*(begins[i]), comp...);

		pool[i - 1] = std::thread(std::merge<InputIterator1, 
								InputIterator2, OutputIterator, Compare...>, 
								begins[i-1],
								begins[i],
								ends[i-1],
								ends[i],
								result + offset,
								comp...);

		offset += (ends[i] - ends[i - 1]) + (begins[i] - begins[i - 1]);

	}

		pool[cores - 1] = std::thread(std::merge<InputIterator1, 
								InputIterator2, OutputIterator, Compare...>, 
								begins[cores - 1],
								last1,
								ends[cores - 1],
								last2,
								result + offset,
								comp...);
	return result;
}

}

#endif