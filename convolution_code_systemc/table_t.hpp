#ifndef TABLE_T_HPP
#define TABLE_T_HPP

#include <iostream>
#include <vector>

using namespace std;

// test code for (2, 1, 4)
#define N 2  //num of output bits
#define K 1  //num of input bits
#define M 4  //num of memory registers
#define L 3  //constraint length

// lookup table definition
class table_t
{
  public:
    
    bool         bit_in;
    vector<bool> state_current;
    vector<bool> bit_out;
    vector<bool> state_next;
    
    table_t() : bit_in(0), state_current(M-1), bit_out(N), state_next(M-1) {}
    
    table_t(bool tmp1, vector<bool> tmp2, vector<bool> tmp3, vector<bool> tmp4 )
           : bit_in(0), state_current(M-1), bit_out(N), state_next(M-1)
    {
      bit_in         = tmp1;
      state_current  = tmp2;
      bit_out        = tmp3;
      state_next     = tmp4;
    }
    
    void print()
    {
      cout << "   ";
      cout << bit_in;
      cout << " |";
      
      for(auto it = state_current.begin(); it != state_current.end(); ++it)
      {  cout << " " << (*it);  }
      
      cout << " |";
      
      for(auto it = bit_out.begin(); it != bit_out.end(); ++it)
      {  cout << " " << (*it);  }
      
      cout << " |";
      
      for(auto it = state_next.begin(); it != state_next.end(); ++it)
      {  cout << " " << (*it);  }
      
      cout << endl;
    }
    
    //increase 1 bit for the current state
    void move_current_state()
    {
      for(auto it = state_current.end(); it != ( --state_current.begin() ); )
      {
          if(*it)  {  *it = 0;  it--;  }
        
          else //aply and exit for loop
            {  *it = 1;  return;  }
      }
    }
    
    // calculate the next state
    // shift the state variable one bit to the right, and then take the input bit and
    // put it in the start bit
    void calculate_next_state()
    {
      state_next[0] = bit_in;
      
      for(int index = 1; index < state_current.size(); ++index)
      {
        state_next[index] = state_current[index-1];
      }
    }
    
    // use generation polynomoid to calculate the lookup table
    void calculate_output()
    {
      bit_out[0] = bit_in ^ state_current[0] ^ state_current[1] ^ state_current[2];
      bit_out[1] = bit_in ^ state_current[0] ^ state_current[2];
    }
};

#endif

