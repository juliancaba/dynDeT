// -*- coding: utf-8; mode: c++; tab-width: 4 -*-

#include "utils.h"

#include <fstream>
#include <sstream>
#include <stdlib.h>

using namespace std;


char *
word2hex(string word, bool littleEndian)
{
  char *buffer = new char[WORD_SIZE];
  stringstream wordHex; 
  int b0, b1, b2, b3;
    
  stringstream c0 (word.substr(0,2));
  stringstream c1 (word.substr(2,2));
  stringstream c2 (word.substr(4,2));
  stringstream c3 (word.substr(6,2));

  c0 >> std::hex >> b0;
  c1 >> std::hex >> b1;
  c2 >> std::hex >> b2;
  c3 >> std::hex >> b3;
      
  
  if (littleEndian){	
	buffer[0] = (unsigned char)(int) b3;
	buffer[1] = (unsigned char)(int) b2;
	buffer[2]= (unsigned char)(int) b1;
	buffer[3] = (unsigned char)(int) b0;
  }else {
	buffer[3] = (unsigned char)(int) b3;
	buffer[2] = (unsigned char)(int) b2;
	buffer[1]= (unsigned char)(int) b1;
	buffer[0] = (unsigned char)(int) b0;
  }

  return buffer;
}


string
hex2word(char *buffer, bool littleEndian)
{
  string word = DUMMY_WORD;
  stringstream b0, b1, b2, b3; 


  if (littleEndian){
	b3 << hex << (int)(unsigned char)buffer[0];
	b2 << hex << (int)(unsigned char)buffer[1];
	b1 << hex << (int)(unsigned char)buffer[2];
	b0 << hex << (int)(unsigned char)buffer[3];
  }
  else {
	b0 << hex << (int)(unsigned char)buffer[0];
	b1 << hex << (int)(unsigned char)buffer[1];
	b2 << hex << (int)(unsigned char)buffer[2];
	b3 << hex << (int)(unsigned char)buffer[3]; 
  }

  SLR(&word, b0.str());
  SLR(&word, b1.str());
  SLR(&word, b2.str());
  SLR(&word, b3.str());

  return word;
}


void
SLR(string *word, string byte)
{
  stringstream tmp; 


  if (byte.size() == 1)
    tmp << (*word).substr(2,7) << "0" << byte;
  else 
    tmp << (*word).substr(2,7) << byte;

  *word = tmp.str();
}


int
getSeekEndHeader(string bitstream, bool littleEndian)
{
  int seek = 0;
  char *byte = new char[1]; 
  string word = DUMMY_WORD;
  

  ifstream bitstreamFile(bitstream.c_str(), ios::in | ios::binary);

  if (bitstreamFile.is_open()){
    while (!bitstreamFile.eof()){
      seek++;

      bitstreamFile.read(byte,1);
      stringstream b; 
      b << hex << (int)(unsigned char)byte[0];
      SLR(&word, b.str());

      if (word == SYNC_WORD_LE && littleEndian) break;
      else if (word == SYNC_WORD) break;
    }
    bitstreamFile.close();
  }
  else{
    cerr << "Bitstream file not found!!!" << endl;
    exit(1);
  } 

  delete byte;

  return seek;
}


unsigned int
hex2uint(const char *chex)
{
  unsigned int result;

  std::stringstream saux;
  saux << std::hex << chex;
  saux >> result; 

  return result;
}
