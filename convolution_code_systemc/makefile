# system library directory, please use your own systemc library directory
LIB := -L/home/cosailer/projects/systemc-2.3.1/lib-linux -L.

# systemc include directory, please use your own systemc include directory
INC := -I/home/cosailer/projects/systemc-2.3.1/include -I.

# define compiler and flags
CXX := g++
CXXFLAGS := $(INC) $(LIB) -lsystemc -std=c++11

######################## systemc setup ####################

default: conv_code
	
conv_code: conv_code.cpp bit_generator_t.o bit_monitor_t.o conv_encoder_t.o viterbi_decoder_t.o bit_error_t.o
	$(CXX) conv_code.cpp -o conv_code $(CXXFLAGS) bit_generator_t.o bit_monitor_t.o conv_encoder_t.o viterbi_decoder_t.o bit_error_t.o
	
bit_generator_t.o: bit_generator_t.cpp
	$(CXX) -c bit_generator_t.cpp -o bit_generator_t.o $(CXXFLAGS)
	
bit_monitor_t.o: bit_generator_t.cpp
	$(CXX) -c bit_monitor_t.cpp -o bit_monitor_t.o $(CXXFLAGS)
	
conv_encoder_t.o: conv_encoder_t.cpp
	$(CXX) -c conv_encoder_t.cpp -o conv_encoder_t.o $(CXXFLAGS)
	
viterbi_decoder_t.o: viterbi_decoder_t.cpp
	$(CXX) -c viterbi_decoder_t.cpp -o viterbi_decoder_t.o $(CXXFLAGS)
	
bit_error_t.o: bit_error_t.cpp
	$(CXX) -c bit_error_t.cpp -o bit_error_t.o $(CXXFLAGS)
	
clean:
	$(RM) conv_code bit_generator_t.o bit_monitor_t.o conv_encoder_t.o viterbi_decoder_t.o bit_error_t.o

