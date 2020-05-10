#ifndef BIT_GENERATOR_T_HPP
#define BIT_GENERATOR_T_HPP

#include <systemc.h>
#include <vector>

using namespace std; 

SC_MODULE(bit_generator_t)
{
  // bit generated
  sc_out<bool> bit_out;
  
  vector<bool> test_bit;
  long  time_counter;
  
  bool mute;
  
  // clock event to send random bit
  sc_event clock_tick;
  
  SC_HAS_PROCESS(bit_generator_t);
  
  bit_generator_t(sc_module_name name, bool mute_module = 0);
  
  void generate_method();
  
  void generate_trigger_thread();
};

#endif

