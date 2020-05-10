#ifndef BIT_ERROR_T_HPP
#define BIT_ERROR_T_HPP

#include <systemc.h>
#include <vector>

using namespace std; 

SC_MODULE(bit_error_t)
{
  // 2 bits input
  sc_in<bool> bit1_in;
  sc_in<bool> bit2_in;
  
  // 2 bits output
  sc_out<bool> bit1_out;
  sc_out<bool> bit2_out;
  
  // variable to add errors
  int  bit_distance;
  int  bit_counter;
  
  bool mute;
  
  // switch on/off error addition
  bool error_mode_on;
  
  // clock event to send random bit
  sc_event clock_tick;
  
  SC_HAS_PROCESS(bit_error_t);
  
  bit_error_t(sc_module_name name, int distance_init = 5 , bool mute_module = 0);
  
  void bit_error_method();
  
  void bit_error_trigger_thread();
};

#endif

