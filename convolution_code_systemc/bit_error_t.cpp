#include "bit_error_t.hpp"

//constructor
bit_error_t::bit_error_t( sc_module_name name, int distance_init, bool mute_module )
                 :sc_module(name), bit_distance(distance_init), bit_counter(0), error_mode_on(1), mute(mute_module)
{
  cout << ">> time = " << sc_time_stamp() << " bit_error: " << name;
  cout << " created, error distance = " << bit_distance << endl;
  
  SC_THREAD(bit_error_trigger_thread);
  SC_METHOD(bit_error_method);
  dont_initialize();
  
  sensitive << clock_tick;
}

void bit_error_t::bit_error_method()
{
  //cout << ">> time = " << sc_time_stamp() << ", " << name() << " in : " <<  << endl;
  
  // add one bit error for ecery [bit_distance] bits
  if( ( bit_counter == bit_distance )&&error_mode_on )
  {
    // randomly chose a bit and apply error
    bool tmp = rand()%2;
    
    // add some error
    if(!mute)
    {
      cout << endl << "<< time = " << sc_time_stamp();
      cout << ", WARNING !! add one bit error at bit " << tmp << endl << endl;
    }
    
    if(tmp)
    {  bit1_out = !bit1_in;
       bit2_out = bit2_in;  }
    else
    {  bit1_out = bit1_in;
       bit2_out = !bit2_in;  }
    
    bit_counter = 0; //reset bit count
  }
  else
  {
    // no error introduced
    bit1_out->write(bit1_in);
    bit2_out->write(bit2_in);
  }
}

void bit_error_t::bit_error_trigger_thread()
{
  while(true)
  {
    wait(1, SC_SEC);
    bit_counter++;
    clock_tick.notify();
  }
}

