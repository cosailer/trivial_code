#include "bit_generator_t.hpp"

//constructor
bit_generator_t::bit_generator_t( sc_module_name name, bool mute_module)
                 :sc_module(name), test_bit(100), time_counter(0), mute(mute_module)
{
  cout << ">> time = " << sc_time_stamp() << " bit_generator: " << name << " created" << endl;
  
  
  //test_bit = { 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, };
  /*
  cout << ">> bit stream = ";
  for(auto it = test_bit.begin(); it != test_bit.end(); it++)
  {
    cout << (*it);
  }
  cout << endl;
  */
  
  SC_THREAD(generate_trigger_thread);
  SC_METHOD(generate_method);
  //dont_initialize();
  
  sensitive << clock_tick;
}

void bit_generator_t::generate_method()
{
  bool tmp = rand()%2;
  //bool tmp = test_bit[time_counter];
  
  if(!mute)
  {
    cout << ">> time = " << sc_time_stamp() << ", " << name() << " out : " << tmp << endl;
  }
  
  //sleep(1);
  bit_out->write(tmp);
}

void bit_generator_t::generate_trigger_thread()
{
  while(true)
  {
    wait(1, SC_SEC);
    //time_counter++;
    clock_tick.notify();
  }
}

