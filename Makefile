CXXFLAGS=-pthread -std=c++0x

all: parallel_sort_test parallel_copy_test

parallel_sort_test: parallel_sort_test.cpp
	g++ $^ $(CXXFLAGS) -o $@

parallel_copy_test: parallel_copy_test.cpp
	g++ $^ $(CXXFLAGS) -o $@

clean:
	rm -f *_test