// find_first_of algorithm example
#include <iostream>     // std::cout
#include <algorithm>    // std::generate
#include <vector>       // std::vector
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <parallel/algorithm>

// function generator:
//int RandomNumber () { return (std::rand()%100); }

//class generator:
struct c_unique {
  int current;
  c_unique() {current=0;}
  int operator()() {return ++current;}
} UniqueNumber;

std::vector<int> myvector(1000*1000*100);

#define ITEM 9999999

int main () {
//    std::vector<int>::iterator it;
    int items[] = {ITEM - 3, ITEM + 5, ITEM - 3};
    std::vector<int> second(items, items+2);
    fprintf(stderr, "omp num threads %d\n", omp_get_max_threads());
//    std::srand ( unsigned ( std::time(0) ) );
  
  //  __gnu_parallel::generate (myvector.begin(), myvector.end(), RandomNumber);
  
  //  std::cout << "myvector contains:";
  //  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
  //    std::cout << ' ' << *it;
  //  std::cout << '\n';
  
    generate (myvector.begin(), myvector.end(), UniqueNumber);
    __gnu_parallel::find_first_of (myvector.begin(), myvector.end(), second.begin(), second.end());
//    std::cout << "Found" << *it << "\n";
  
  //  std::cout << "myvector contains:";
  //  for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
  //    std::cout << ' ' << *it;
  //  std::cout << '\n';
   
    return 0;
}
