#ifndef CONV_ENCODER_T_HPP
#define CONV_ENCODER_T_HPP

#include <systemc.h>
#include <iostream>
#include <vector>
#include <cmath>
#include "table_t.hpp"

SC_MODULE(conv_encoder_t)
{
  // bit generated
  sc_in<bool> bit_in;
  
  // 2 encoded bits output
  sc_out<bool> bit1_out;
  sc_out<bool> bit2_out;
  
  bool mute;
  
  // clock event to receive bit and output 2 bit
  sc_event clock_tick;
  
  vector<table_t>    lookup_table;
  vector<bool>       encode_state;
  int verbose;
  
  SC_HAS_PROCESS(conv_encoder_t);
  
  conv_encoder_t(sc_module_name name, bool mute_module = 0);
  
  void encode_method();
  
  ~conv_encoder_t();
  
  void create_table();
  
  void conv_encoder_trigger_thread();
  
};

#endif

