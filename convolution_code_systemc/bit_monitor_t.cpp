#include "bit_monitor_t.hpp"

using namespace std;

//constructor
bit_monitor_t::bit_monitor_t( sc_module_name name, bool mute_module)
          :sc_module(name), mute(mute_module)
{ 
  cout << ">> time = " << sc_time_stamp() << " bit_monitor: " << name << " created" << endl;
  
  SC_THREAD(monitor_trigger_thread);
  SC_METHOD(monitor_method);
  //dont_initialize();
  sensitive << clock_tick;
}

void bit_monitor_t::monitor_method()
{
  cout << ">> time = " << sc_time_stamp() << ", " << name() << " in : " << bit_in << endl;
}

void bit_monitor_t::monitor_trigger_thread()
{
  while(true)
  {
    wait(1, SC_SEC);
    clock_tick.notify();
  }
}


