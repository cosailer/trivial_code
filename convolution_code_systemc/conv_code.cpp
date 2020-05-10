#include <systemc.h>
#include "bit_generator_t.hpp"
#include "bit_monitor_t.hpp"
#include "conv_encoder_t.hpp"
#include "viterbi_decoder_t.hpp"
#include "bit_error_t.hpp"

using namespace std;

void help()
{
  cout << endl;
  cout << ">> convolution code demo v0.1" << endl;
  cout << "          by CoSailer 2016.JUL.04" << endl;
  cout << "   useage:" << endl;
  cout << "          conv_code [simulation time] [error distance] [mute]" << endl;
  cout << endl;
  cout << "   simulation time: integer, specify how long the simulation run" << endl;
  cout << "   error distance: specify how many bits between an error" << endl;
  cout << "   mute : m, toggle mute for modules for less output" << endl;
  cout << endl;
}

int sc_main(int argc, char* argv[])
{
  cout << endl << endl << "Viterbi Decoder SystemC Demo v0.1" << endl;
  cout << "by CoSailer, 2016/07/05" << endl << endl;
  
  bool mute_module = 0;      // toogle verbose
  int simulation_time = 10;   // number of input bits before encoder
  int error_distance = 5;    // number of random errors before decoder
  
  // command line pasring
  if( argc ==1 )
  {  cout << ">> using default settings" << endl;  }
  
  else if( argc ==2 )
  {  simulation_time = atoi(argv[1]);  }
  
  else if( argc == 3 )
  {  simulation_time = atoi(argv[1]);
     error_distance = atoi(argv[2]); }
  
  else if( argc == 4 )
  {
    simulation_time = atoi(argv[1]);
    error_distance = atoi(argv[2]);
    
    if( *argv[3] == 'm' ) { mute_module = 1; }
    else {  help();  exit(0);  }
  }
  else {  help();  exit(0);  }
  
  cout << ">> num_of_bit = "<< simulation_time << ",  num_of_error = " << error_distance;
  cout << ",  mute = " << mute_module << endl;
  
  // create 4 channels
  sc_signal<bool> original_bit;
  sc_signal<bool> encode_bit1;
  sc_signal<bool> encode_bit2;
  
  sc_signal<bool> error_bit1;
  sc_signal<bool> error_bit2;
  //sc_signal<bool> decode_bit;
  
  
  // set simulation time
  sc_time sim_time(simulation_time, SC_SEC);
  
  // create modules---------------------------------------------
  
  // random bit generator
  bit_generator_t G1("Generator_1", mute_module);
  
  // convolution code encoder
  conv_encoder_t E1("Encoder_1", mute_module);
  
  // viterbi encoder, 1: muted by default, otherwise too much output, only for debug
  viterbi_decoder_t V1("Viterbi_1", 1);
  //V1.verbose = 1;
  
  // bit error module, 5: error distance
  bit_error_t B1("Bit_Error_1", error_distance, mute_module);
  
  // output bit monitor
  //bit_monitor_t  M1("Monitor_1");
  //bit_monitor_t  M2("Monitor_2");
  
  //connect modules ---------------------------------------------
  G1(original_bit);
  E1(original_bit, encode_bit1, encode_bit2);
  B1(encode_bit1, encode_bit2, error_bit1, error_bit2);
  V1(error_bit1, error_bit2 );
  //M1(encode_bit1);
  //M2(encode_bit2);
  
  sc_start(sim_time);
  
  return 0;
}

