#ifndef BIT_MONITOR_T_HPP
#define BIT_MONITOR_T_HPP

#include <systemc.h>
#include <iostream>

SC_MODULE(bit_monitor_t)
{
  // bit generated
  sc_in<bool> bit_in;
  
  bool mute;
  
  // clock to update display input bit
  sc_event clock_tick;
  
  SC_HAS_PROCESS(bit_monitor_t);
  
  bit_monitor_t(sc_module_name name, bool mute_module = 0);
  
  void monitor_method();
  void monitor_trigger_thread();
  
};

#endif

