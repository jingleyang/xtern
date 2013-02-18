// transform algorithm example
#include <iostream>     // std::cout
#include <algorithm>    // std::generate
#include <vector>       // std::vector
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <parallel/algorithm>

// function generator:
int RandomNumber () { return (std::rand()%100); }

int myfunc(int i) {
    return ++i;
}

//class generator:
//struct c_unique {
//  int current;
//  c_unique() {current=0;}
//  int operator()() {return ++current;}
//} UniqueNumber;

std::vector<int> myvector(1000*1000*100);
std::vector<int> result(1000*1000*100);
//std::vector<int> myvector(10);
//std::vector<int> result(10);

//#define ITEM 9999999
//#define ITEM 56

int main () {
//   int a = 0;
    fprintf(stderr, "omp num threads %d\n", omp_get_max_threads());
//    std::srand ( unsigned ( std::time(0) ) );
//    __gnu_parallel::generate (myvector.begin(), myvector.end(), RandomNumber);
  
//    generate (myvector.begin(), myvector.end(), UniqueNumber, __gnu_parallel::sequential_tag());

//    std::cout << "myvector contains:";
//    for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
//      std::cout << ' ' << *it;
//    std::cout << '\n';
  
    __gnu_parallel::transform(myvector.begin(), myvector.end(), result.begin(), myfunc);
  
//    std::cout << "myvector contains:";
//    for (std::vector<int>::iterator it=result.begin(); it!=result.end(); ++it)
//      std::cout << ' ' << *it;
//    std::cout << '\n';
   
    return 0;
}
