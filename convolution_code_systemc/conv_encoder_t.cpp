#include "conv_encoder_t.hpp"

using namespace std;

//constructor
conv_encoder_t::conv_encoder_t( sc_module_name name, bool mute_module)
            :sc_module(name), encode_state(M-1), verbose(0), mute(mute_module)
{ 
  cout << ">> time = " << sc_time_stamp() << " conv_encoder " << name << " created" << endl;
  
  // setup encoder default state
  encode_state = { 0, 0, 0 };
  create_table();
  
  SC_THREAD(conv_encoder_trigger_thread);
  SC_METHOD(encode_method);
  dont_initialize();
  sensitive << clock_tick;
}

conv_encoder_t::~conv_encoder_t()
{
  lookup_table.clear();
  encode_state.clear();
}

void conv_encoder_t::conv_encoder_trigger_thread()
{
  while(true)
  {
    wait(1, SC_SEC);
    clock_tick.notify();
  }
}

void conv_encoder_t::encode_method()
{
  //compare register with the lookup table to get output bits and state
  for(auto it = lookup_table.begin(); it != lookup_table.end(); ++it)
  {
    //if match found
    if( ( bit_in == it->bit_in ) && (encode_state == it->state_current) )
    {
      
      if(!mute)
      {
        cout << ">> time = " << sc_time_stamp() << ", " << name();
        cout << ",  in = " << bit_in;
        cout << ",  out = " << it->bit_out[0] << it->bit_out[1];
        cout << ",  state = " << encode_state[1];
        cout << encode_state[2] << encode_state[3] << endl;
      }
      
      //update state
      encode_state = it->state_next;
      
      //write to output
      bit1_out->write(it->bit_out[0]);
      bit2_out->write(it->bit_out[1]);
      
      break; // break for next bit
    }
  }
}

// create lookup table
void conv_encoder_t::create_table()
{
  if(!mute) { cout << "<< populating lookup table" << endl; }
  
  table_t table_entry(0, { 0, 0, 0 }, { 0, 0, }, { 0, 0, 0, });
  
  int num_table_entry = pow(2, M);

  for(int i = 0; i < num_table_entry; ++i)
  {
    //update input bit
    table_entry.bit_in = i%2;
    
    //update current state
    table_entry.move_current_state();
    
    //update the next state with the updated input bit
    table_entry.calculate_next_state();
    
    //update output bits
    table_entry.calculate_output();
    
    // push the modified table entry into the actual lookup table
    lookup_table.push_back( table_entry );
  }
  
  /* 
  // or you can set up the lookup table manually
  lookup_table.push_back( table_t( 0, { 0, 0, 0 }, { 0, 0, }, { 0, 0, 0, } ) );
  lookup_table.push_back( table_t( 1, { 0, 0, 0 }, { 1, 1, }, { 1, 0, 0, } ) );
  lookup_table.push_back( table_t( 0, { 0, 0, 1 }, { 1, 1, }, { 0, 0, 0, } ) );
  lookup_table.push_back( table_t( 1, { 0, 0, 1 }, { 0, 0, }, { 1, 0, 0, } ) );
  lookup_table.push_back( table_t( 0, { 0, 1, 0 }, { 1, 0, }, { 0, 0, 1, } ) );
  lookup_table.push_back( table_t( 1, { 0, 1, 0 }, { 0, 1, }, { 1, 0, 1, } ) );
  lookup_table.push_back( table_t( 0, { 0, 1, 1 }, { 0, 1, }, { 0, 0, 1, } ) );
  lookup_table.push_back( table_t( 1, { 0, 1, 1 }, { 1, 0, }, { 1, 0, 1, } ) );
  lookup_table.push_back( table_t( 0, { 1, 0, 0 }, { 1, 1, }, { 0, 1, 0, } ) );
  lookup_table.push_back( table_t( 1, { 1, 0, 0 }, { 0, 0, }, { 1, 1, 0, } ) );
  lookup_table.push_back( table_t( 0, { 1, 0, 1 }, { 0, 0, }, { 0, 1, 0, } ) );
  lookup_table.push_back( table_t( 1, { 1, 0, 1 }, { 1, 1, }, { 1, 1, 0, } ) );
  lookup_table.push_back( table_t( 0, { 1, 1, 0 }, { 0, 1, }, { 0, 1, 1, } ) );
  lookup_table.push_back( table_t( 1, { 1, 1, 0 }, { 1, 0, }, { 1, 1, 1, } ) );
  lookup_table.push_back( table_t( 0, { 1, 1, 1 }, { 1, 0, }, { 0, 1, 1, } ) );
  lookup_table.push_back( table_t( 1, { 1, 1, 1 }, { 0, 1, }, { 1, 1, 1, } ) );
  */
  
}


