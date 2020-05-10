#include "viterbi_decoder_t.hpp"

using namespace std;

//constructor
viterbi_decoder_t::viterbi_decoder_t( sc_module_name name, bool mute_module)
            :sc_module(name), mute(mute_module)
{
  cout << ">> time = " << sc_time_stamp() << " viterbi_decoder: " << name << " created" << endl;
  
  create_table();
  
  if(!mute)
  {
    cout << ">> create the first decode path" << endl;
  }
      
  path_t first_path;
  first_path.path_state = { 0, 0, 0 };
  first_path.path_metric = 0;
  first_path.alive = 1;
      
  path_table.push_back( first_path );
  
  SC_THREAD(decoder_trigger_thread);
  SC_METHOD(decode_method);
  dont_initialize();
  sensitive << clock_tick;
}

viterbi_decoder_t::~viterbi_decoder_t()
{
  lookup_table.clear();
  path_table.clear();
  path_table_tmp.clear();
  result.clear();
}

void viterbi_decoder_t::decoder_trigger_thread()
{
  while(true)
  {
    wait(1, SC_SEC);
    clock_tick.notify();
  }
}

void viterbi_decoder_t::decode_method()
{
  cout << ">> time = " << sc_time_stamp() << ", " << name() << ",  bits_in = " << bit1_in << bit2_in;
  cout << ",  num of path = " << path_table.size() << endl;
 
  
  //print_path_table();
  
  // loop through each decode path
  for(auto it2 = path_table.begin(); it2 != path_table.end(); ++it2)
  {
    if(!mute)
    {
      cout << "<< it2 : ";
      it2->print();
      print_path_table();
    }
      
    // loop through the lookup table again
    // to create possible new path for each state, if state matches
    for(auto it3 = lookup_table.begin(); it3 != lookup_table.end(); ++it3)
    {
      path_t tmp = *it2; //create tmp path from it2
  
      // if the state matches, create new paths
      if(tmp.path_state == it3->state_current)
      {
        if(!mute)
        {
          cout << "<< new path,";
          cout << "   OB = " << it3->bit_in;
          cout << ",  CS = " << it3->state_current[0] << it3->state_current[1] << it3->state_current[2];
          cout << ",  NS = " << it3->state_next[0] << it3->state_next[1] << it3->state_next[2];
          cout << ",  PMO = " << tmp.path_metric;
        }
        
        //update original bit for new path
        tmp.original_bit.push_back( it3->bit_in );
          
        //update state
        tmp.path_state = it3->state_next;
          
        //update path metric
        int metric = 0;
        if(bit1_in == it3->bit_out[0]) { metric++; }
        if(bit2_in == it3->bit_out[1]) { metric++; }
          
        tmp.path_metric += metric;
          
        if(!mute) { cout << ",  PMN = " << tmp.path_metric << endl; }
          
        tmp.alive = 1;
        
        //update temporary path table
        path_table_tmp.push_back(tmp);
      }
    }
    
    //kill current decode path
    it2->alive = 0;
  }
  
  // update path table
  update_path_table();
  
  //print_path_table();
  
  // print temporary result
  extract();
  print_result();
}

// create lookup table
void viterbi_decoder_t::create_table()
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

// print lookup table
void viterbi_decoder_t::print_lookup_table()
{
  cout << endl;
  cout << "   look up table: " << endl;
  
  for(auto it = lookup_table.begin(); it != lookup_table.end(); ++it)
  {  it->print();  }
  
  cout << endl << endl;
}
    
// print lookup table
void viterbi_decoder_t::print_path_table()
{
  cout << endl;
  cout << "   path table: " << path_table.size() << endl;
  
  for(auto it = path_table.begin(); it != path_table.end(); ++it)
  {  it->print();  }
    
  cout << endl << endl;
}
    
// method to compare convering nodes
// and remove all the dead paths
void viterbi_decoder_t::update_path_table()
{
  if(!mute) { cout << "<< updating path table" << endl; }
  
  remove_dead_path();
  
  copy_tmp_path_table();
  
  kill_convering_node();
  
  remove_dead_path();
}
    
void viterbi_decoder_t::copy_tmp_path_table()
{
  //copy all path from path_table_tmp to path_table
  for(auto it = path_table_tmp.begin(); it != path_table_tmp.end(); ++it)
  {
    path_table.push_back(*it);
  }
      
  //clear temporary path_table_tmp
  path_table_tmp.clear();
}

void viterbi_decoder_t::kill_convering_node()
{
  //remove nodes convering to the same node
  auto before_end = path_table.end();
  --before_end;
      
  for(auto it = path_table.begin(); it != before_end; it++)
  {
    auto after_first = it;
    after_first++;
  
    for(auto it2 = after_first; it2 != path_table.end(); it2++)
    {
      // if two nodes are convering to the same state
      if( it->path_state == it2->path_state )
      {
        if(!mute)
        {
          cout << "<< paths convering !!" << endl;
          cout << "   path 1 : ";  it->print();
          cout << "   path 2 : ";  it2->print();
        }
              
        // kill the one with lower metric
        // if same, do nothing
        if(it->path_metric < it2->path_metric)
        {
          it->alive = 0;
          if(!mute) { cout << "<< path 1 killed" << endl;};
        }
              
        else if(it->path_metric > it2->path_metric)
        {
          it2->alive = 0;
          if(!mute) { cout << "<< path 2 killed" << endl;}
        }
            
        else
        { if(!mute) { cout << "<< both path live" << endl;} }
      }
    }
  }
}
    
void viterbi_decoder_t::remove_dead_path()
{
  // remove dead paths
  for(auto it = path_table.begin(); it != path_table.end(); )
  {
    if( it->alive == 0)
    {
      it = path_table.erase(it);
      if(!mute) { cout << "<< path removed" << endl; }
    }
        
    else { ++it; }
  }
}
    
void viterbi_decoder_t::extract()
{
  result.clear();
  
  if( path_table.size() == 0 )
    { cout << ">> no result found !" << endl; return ;}
  else  // put the first item into the result
    { result.push_back( path_table[0] ); }
  
  // compare the first with the second, so start with the second in the table
  for(auto it = (++path_table.begin()); it != path_table.end(); ++it)
  {
    // if a better path is found, clear the existing result table
    // then push the better path into the result 
    if( result[0].path_metric < it->path_metric )
    {
      result.clear();
      result.push_back(*it);
    }
    // if a path with same metric is found
    // just push the same path into the result 
    else if( result[0].path_metric == it->path_metric )
    {
      result.push_back(*it);
    }
  }
  
  // just choose the first result and write to decoder output
}

void viterbi_decoder_t::print_result()
{
  cout << endl << ">> current decode result: " << endl;
  
  if( result.size() == 1 )
    { cout << "   most possible decode path :" << endl; }
  else
    { cout << "   WARNING: more than 1 most possible decode path !" << endl; }
  
  for(auto it = result.begin(); it != result.end(); ++it)
  {
    it->print();
  }
  
  cout << endl;
  path_table_tmp.clear();
}

