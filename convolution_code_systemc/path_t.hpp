#ifndef PATH_T_HPP
#define PATH_T_HPP

#include <iostream>
#include <vector>

using namespace std;

// test code for (2, 1, 4)
#define N 2  //num of output bits
#define K 1  //num of input bits
#define M 4  //num of memory registers
#define L 3  //constraint length


class path_t
{
  public:
  
    vector<bool>  original_bit;  // store temporary decoded bits
    vector<bool>  path_state;    // current state
    int    path_metric;          // hamming metric ?
    bool   alive;                // whether the path is alive, 1 : fowllow, 0 : delete
    
    path_t() : path_state(M-1), alive(1) {}
    
    path_t(vector<bool> tmp1, vector<bool> tmp2, bool tmp3, int tmp4) :
           path_state(M-1), alive(1), path_metric(0)
    {
      original_bit = tmp1;
      path_state   = tmp2;
      alive        = tmp3;
      path_metric  = tmp4;
    }
    
    void print()
    {
      cout << "   DBit : ";
      for(auto it = original_bit.begin(); it != original_bit.end(); ++it)
      {
        cout << (*it);
      }
      
      cout << "   PS : ";
      for(auto it = path_state.begin(); it != path_state.end(); ++it)
      {
        cout << (*it);
      }
      
      cout << "   ALIV = " << alive;
      cout << "   PM = " << path_metric << endl;
    }
};

#endif

