#include <iostream>
#include <vector>
#include <bitset>

//wifi password: D8vwB4xlZl8wmkSr8SwnSVAJ

// ASCII index
// "0-9": 48 - 57
// "A-Z": 65 - 90
// "a-z": 97 - 122
// "+" : 43
// "/" : 47

using namespace std;

// int here
class base64_t
{
  public:
  
    vector<char> lookup_table;
    vector<char> input;
    vector<char> output;
    
    uint32_t word_buffer;
    
    uint32_t in_word_1;
    uint32_t in_word_2;
    uint32_t in_word_3;
    
    uint32_t out_index_1;
    uint32_t out_index_2;
    uint32_t out_index_3;
    uint32_t out_index_4;
    
    base64_t() : in_word_1(0), in_word_2(0), in_word_3(0)
    {
      create_table();
    }
    
    void create_table()
    {
      // populating base64 lookup table
      for(int i = 0; i < 26; ++i)
      {
        lookup_table.push_back( (char)(i+65) );
      }
      
      for(int i = 26; i < 52; ++i)
      {
        lookup_table.push_back( (char)(i+71) );
      }
      
      for(int i = 52; i < 62; ++i)
      {
        lookup_table.push_back( (char)(i-4) );
      }
      
      lookup_table.push_back('+');
      lookup_table.push_back('/');
    }
    
    // convert original input char array to char vector
    void convert_input(char *buffer)
    {
      int i = 0;
  
      while( buffer[i] != '\0' )
      {
        input.push_back( buffer[i] );
        i++;
      }
    }
    
    // convert encoded output char array to char vector
    void convert_output(char *buffer)
    {
      int i = 0;
  
      while( buffer[i] != '\0' )
      {
        output.push_back( buffer[i] );
        i++;
      }
    }
    
    void display_table()
    { 
      for( int i = 0; i < lookup_table.size(); ++i)
      {
        cout << " index = " << i << ", value = " << lookup_table[i] << endl;
      }
    }
    
    void display_input()
    {
      cout << "original stream:" << endl;
      
      for(auto it = input.begin(); it != input.end(); ++it)
      {  cout << *it;  }
      
      cout << endl << endl;
    }
    
    void display_output()
    {
      cout << "base64 encoded stream:" << endl;
      
      for(auto it = output.begin(); it != output.end(); ++it)
      {  cout << *it;  }
      
      cout << endl << endl;
    }
    
    void encode()
    {
      //input padding
      if( input.size()%3 == 2 )
        { input.push_back(0); }
      else if( input.size()%3 == 1 )
        { input.push_back(0); input.push_back(0); }
      
      // for each 3 input bytes in the input
      // loop through encode to create 4 output bytes
      for(int i = 0; i < input.size()-2; i+=3)
      {
        // shift input bytes to 3x size
        in_word_1 = input[ i ] << 16;
        in_word_2 = input[i+1] << 8;
        in_word_3 = input[i+2];
        
        // combine the 3 input words to a bigger binary stream 
        word_buffer = in_word_1 | in_word_2 | in_word_3;
        
        // get the individual 4 sub output words
        //out_index_1 = (word_buffer & 0b111111000000000000000000) >> 18;
        //out_index_2 = (word_buffer & 0b000000111111000000000000) >> 12;
        //out_index_3 = (word_buffer & 0b000000000000111111000000) >> 6;
        //out_index_4 = (word_buffer & 0b000000000000000000111111);
        
        out_index_1 = word_buffer >> 18;
        out_index_2 = (word_buffer & 0x3f000) >> 12;
        out_index_3 = (word_buffer & 0xfc0) >> 6;
        out_index_4 = word_buffer & 0x3f;
        
        // convert words into chars and store them in output
        output.push_back( lookup_table[out_index_1] );
        output.push_back( lookup_table[out_index_2] );
        output.push_back( lookup_table[out_index_3] );
        output.push_back( lookup_table[out_index_4] );
      }
      
      // change 'A' to '=' at the end
      if( *(output.rbegin()) == 'A' ) { *(output.rbegin()) = '='; }
      if( *(output.rbegin()+1) == 'A' ) { *(output.rbegin()+1) = '='; }
    }
    
    uint32_t get_index(char tmp)
    {
      for(uint32_t i = 0; i < 64; ++i)
      {
        if( lookup_table[i] == tmp )
        { return i; } 
      }
    }
    
    void decode()
    {
      // input bytes padding
      if( output.size()%4 == 3 )
        { output.push_back('A'); }
      else if( output.size()%4 == 2 )
        { output.push_back('A'); output.push_back('A'); }
      else if( output.size()%4 == 1 )
        { output.pop_back(); cout << ">> illeagal ending, last byte deleted." << endl; }
      
      // change 'A' to '=' at the end
      if( *(output.rbegin()) == '=' ) { *(output.rbegin()) = 'A'; }
      if( *(output.rbegin()+1) == '=' ) { *(output.rbegin()+1) = 'A'; }
      
      // for each 4 encoded words in the stream
      // loop through all words to create 3 original words each time
      for(int i = 0; i < output.size()-3; i+=4)
      {
        //get the original index and shift input bytes to 3x size
        out_index_1 = get_index( output[ i ] ) << 18;
        out_index_2 = get_index( output[i+1] ) << 12;
        out_index_3 = get_index( output[i+2] ) << 6;
        out_index_4 = get_index( output[i+3] );
        
        // combine the 3 input words to a bigger binary stream 
        word_buffer = out_index_1 | out_index_2 | out_index_3 | out_index_4;
        
        // get the individual 4 sub output words
        //in_word_1 = (word_buffer & 0b111111110000000000000000) >> 16;
        //in_word_2 = (word_buffer & 0b000000001111111100000000) >> 8;
        //in_word_3 = (word_buffer & 0b000000000000000011111111);
        
        in_word_1 = word_buffer >> 16;
        in_word_2 = (word_buffer & 0xff00) >> 8;
        in_word_3 = word_buffer & 0xff;
        
        // convert words into chars and store them in output
        input.push_back( char(in_word_1) );
        input.push_back( char(in_word_2) );
        input.push_back( char(in_word_3) );
      }
    }
    
    //empty input and output buffer
    void clear()
    {
      input.clear();
      output.clear();
    }
};

int main()
{
  base64_t base64;
  //base64.display_table();
  
  char in[] = {
  "Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure."
  };
  
  char out[] = {
  "TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1d"
  "CBieSB0aGlzIHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aG"
  "ljaCBpcyBhIGx1c3Qgb2YgdGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2U"
  "gb2YgZGVsaWdodCBpbiB0aGUgY29udGludWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdl"
  "bmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRoZSBzaG9ydCB2ZWhlbWVuY"
  "2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4="
  };
  
  //encode test
  cout << ">> encode test" << endl;
  base64.clear();
  base64.convert_input(in);
  base64.display_input();
  base64.encode();
  base64.display_output();
  
  //decode test
  cout << ">> decode test" << endl;
  base64.clear();
  base64.convert_output(out);
  base64.display_output();
  base64.decode();
  base64.display_input();
  
  cout << endl;
  
  return 0;
}

