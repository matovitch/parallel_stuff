#include "parallel_merge.h"
#include <algorithm>
#include <iostream>
#include <vector>

int main () {
  int first[] = {5,10,15,20,25};
  int second[] = {50,40,30,20,10};
  std::vector<int> v(10);

  std::sort (first,first+5);
  std::sort (second,second+5);
  parallel::merge (first,first+5,second,second+5,v.begin(), 2);

  std::cout << "The resulting vector contains:";
  for (std::vector<int>::iterator it=v.begin(); it!=v.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}