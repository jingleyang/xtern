// merge example
#include <iostream>     // std::cout
#include <algorithm>    // std::set_union, std::sort
#include <vector>       // std::vector
#include <iterator>
#include <parallel/algorithm>

using namespace std;

#define DATA_SIZE 1000*1000*100
//#define DATA_SIZE 4

//int RandomNumber () { return (std::rand()%100); }

//struct c_unique {
//  int current;
//  c_unique() {current=0;}
//  int operator()() {return ++current;}
//} UniqueNumber;

  std::vector<int> first(DATA_SIZE);
  std::vector<int> second(DATA_SIZE);
  std::vector<int> v(DATA_SIZE*2);                     

int main () {
  //  int first[] = {5,10,15,20,25};
  //  int second[] = {50,40,30,20,10};
  //  std::vector<int> first(1000*1000*10);
  //  std::vector<int> second(1000*1000*10);
//    std::vector<int>::iterator it;
      
//    std::srand ( unsigned ( std::time(0) ) );
//    generate (first.begin(), first.end(), RandomNumber);
//    generate (second.begin(), second.end(), RandomNumber);  

//    generate (first.begin(), first.end(), UniqueNumber, __gnu_parallel::sequential_tag());
//    generate (second.begin(), second.end(), UniqueNumber, __gnu_parallel::sequential_tag());
//  
//    sort (first.begin(),first.end(), __gnu_parallel::sequential_tag()); 
//    sort (second.begin(),second.end(), __gnu_parallel::sequential_tag());
  
    fprintf(stderr, "omp num threads %d\n", omp_get_max_threads());
  
//    it = __gnu_parallel::set_union (first.begin(), first.end(), second.begin(), second.end(), v.begin());
    __gnu_parallel::merge(first.begin(), first.end(), second.begin(), second.end(), v.begin());
  
//    v.resize(it-v.begin()); 
//  
//    std::cout << "The union has " << (v.size()) << " elements:\n";
//    for (std::vector<int>::iterator it=v.begin(); it!=v.end(); ++it)
//      std::cout << ' ' << *it;
//    std::cout << '\n';
  
    return 0;
}
