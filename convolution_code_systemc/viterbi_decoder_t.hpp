#ifndef VITERBI_DECODER_T_HPP
#define VITERBI_DECODER_T_HPP

#include <systemc.h>
#include <iostream>
#include <vector>
#include "table_t.hpp"
#include "path_t.hpp"

SC_MODULE(viterbi_decoder_t)
{
  // 2 bit output
  sc_in<bool> bit1_in;
  sc_in<bool> bit2_in;
  
  // bit decoded
  //sc_out<bool> bit_out;
  
  vector<table_t>   lookup_table;
  vector<path_t>    path_table;
  vector<path_t>    path_table_tmp; // for temporary path storage
                                     // before updating the actual path_table
  vector<path_t>    result;  // store the decode path result
  
  // clock event to send random bit
  sc_event clock_tick;
    
  bool mute;
  
  SC_HAS_PROCESS(viterbi_decoder_t);
  
  viterbi_decoder_t(sc_module_name name, bool mute_module = 0);
  
  void decode_method();
  void decoder_trigger_thread();
  
  ~viterbi_decoder_t();
  
  void create_table();
  void print_lookup_table();
  
  void print_path_table();
  void update_path_table();
  void copy_tmp_path_table();
  void kill_convering_node();
  void remove_dead_path();
  
  void extract();
  void print_result();
};

#endif

