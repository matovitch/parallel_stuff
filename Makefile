all:
	g++ parallel_sort_test.cpp -pthread -std=c++0x -o parallel_sort_test

clean:
	rm -f parallel_sort_test