#ifndef PA2_C_
#define PA2_C_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


struct UUID
{
	unsigned int top_top,
				 top_high_mid,
				 top_low_mid,
				 top_bottom,
				 bottom_top,
				 bottom_high_mid,
				 bottom_low_mid,
				 bottom_bottom;
				 
	unsigned int time_low:32; 				//4hexOctets
	unsigned int time_mid:16;				//2hexOctets
	unsigned int time_high_and_version:16;	//2hexOctets
	unsigned int clock_seq_and_reserved:8;	//1hexOctets
	unsigned int clock_seq_low:8;			//1hexOctets
	unsigned int node_high:16;				//2 of 6hexOctets
	unsigned int node_low:32;				//4 of 6hexOctets
				  
				  
};

int SEEDRANDOM(void)
{
	srand((unsigned)time(NULL));
	return 1;
};

unsigned int GETRAND(void)
{
	unsigned int _rand;
	unsigned int _output;
	_output = 0;
	_rand = 0;

	//Grab the top 8 bits from the rand() twice and 
	// OR it with of the _output
	_rand = rand();
	_output = _output | _rand;
    _output &= 0xf0;

	_rand = rand();
	_rand >>= 8;
	_output = _output | _rand;

	return _output;
};

int FILLUUID(struct UUID *_input)
{
	_input->top_top = GETRAND();
	_input->top_high_mid = GETRAND();
	_input->top_low_mid = GETRAND();
	_input->top_bottom = GETRAND();
	_input->bottom_top = GETRAND();
	_input->bottom_high_mid = GETRAND();
	_input->bottom_low_mid = GETRAND();
	_input->bottom_bottom = GETRAND();
	
	return 1;
};

int NORMALIZEUUID(struct UUID *_input)
{
	//time low 32 bits
	_input->time_low = 0;
	_input->time_low = _input->time_low | _input->top_top;
	//shift over for next 16 bits
	_input->time_low <<= 16;
	_input->time_low = _input->time_low | _input->top_high_mid;
	
	//time mid 16 bits
	_input->time_mid = 0;
	_input->time_mid = _input->time_mid | _input->top_low_mid;

	//time low and version 16 bits
	_input->time_high_and_version = 0;
	_input->time_high_and_version = _input->time_high_and_version | _input->top_bottom;
	
	//clock-seq-and-reserved 8 bits
	_input->clock_seq_and_reserved = 0;
	_input->clock_seq_and_reserved = _input->clock_seq_and_reserved | _input->bottom_top;

	_input->bottom_top >>= 8;  //remove 8 used bits
	
	//clock-seq-low 8 bits
	_input->clock_seq_low = 0;
	_input->clock_seq_low = _input->clock_seq_low | _input->bottom_top;
	
	//node 48 bits
	_input->node_high = 0;
	_input->node_high = _input->node_high | _input->bottom_high_mid;

	_input->node_low = 0;
	_input->node_low = _input->node_low | _input->bottom_low_mid;
	//sift node_low for next 16 bits.
	_input->node_low <<=16;
	_input->node_low = _input->node_low | _input->bottom_bottom;
	
	return 1;
};

int STAMPUUID(struct UUID *_input)
{
	//clock_seq_high_and_reserved bit 6-7 = "01"
	// AND with mask to clear reserved bits:
	// 0011 1111b = ?d = 0x3F

	_input->clock_seq_and_reserved = _input->clock_seq_and_reserved & 0x3F;

	// then OR it with 1000 0000 (little endian) to add reserved bits
	// 1000 0000b = 128d = 0x80
	
	_input->clock_seq_and_reserved = _input->clock_seq_and_reserved | 0x80;
	
	//time_high_and_version bit 12-15 = "0100"
	// AND with mask to clear version bits:
	// 0000 1111 1111 1111b = ?d = 0x0FFF

	_input->time_high_and_version = _input->time_high_and_version & 0x0FFF;
	
	// then OR it with 0100000000000000 (little endian) to add version
	// 0100 0000 0000 0000b = 20479d = 0x4000 
	
	_input->time_high_and_version = _input->time_high_and_version | 0x4000;
	
	return 1;
};

int DISPLAYUUID(struct UUID *_input)
{
	printf("%08x-%04x-%04x-%02x%02x-%04x%08x\n\r", 
			_input->time_low, _input->time_mid, _input->time_high_and_version,
			_input->clock_seq_and_reserved, _input->clock_seq_low, 
			_input->node_high, _input->node_low);
	
	return 1;
};

int VERIFYUUID(struct UUID *_input)
{
	/*
	 * Understanding that there is no way to actually verify a UUID
	 * This makes sure that the STAMPUUID funtion performs correctly.	
	*/
	unsigned long temp;
	//does clock_seq_high_and_reserved bit 6-7 = "01"?
	// so and it with 01000000 (little endian)
	// 1000 0000b = 128d = 0x80
	
	temp = 0;	
	temp = _input->clock_seq_and_reserved; 
	temp = temp & 0x80;
	if(temp != 128){return 0;}
	
	//time_high_and_version bit 12-15 = "0100"
	// so and it with 0100000000000000 (little endian)
	// 0100 0000 0000 0000b = 16384d = 0x4000

	temp = 0;
	temp = _input->time_high_and_version;
	temp = temp & 0x4000;
	if(temp != 16384){return 0;}
	
	return 1;	
	
};

#endif
