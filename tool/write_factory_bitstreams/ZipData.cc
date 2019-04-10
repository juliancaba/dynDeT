// -*- coding: utf-8; mode: c++; tab-width: 4 -*-

#include <fstream>
#include <sstream>
#include <cstdlib>

#include <utils.h>
#include <ZipData.hh>
#include <iomanip> 


void
ZipData::zipBitstream()
{
  int zerosCnt = 0;
  string wordCurrent = DUMMY_WORD;
  char *buffer = new char[WORD_SIZE]; 

  ifstream inBitstreamFile(_inputBitstream.c_str(), ios::in | ios::binary);
  ofstream outBitstreamFile(_outputBitstream.c_str(), ios::out | ios::binary);
  
  while (!inBitstreamFile.eof()){
	inBitstreamFile.read(buffer, WORD_SIZE);

	wordCurrent = hex2word(buffer, _littleEndian);

	if (wordCurrent == ZEROS_WORD)	
	  zerosCnt++; 
	else{ 
	  if (zerosCnt == 1)
		outBitstreamFile.write(word2hex(ZEROS_WORD, _littleEndian), WORD_SIZE);
	  else if (zerosCnt == 2){
		outBitstreamFile.write(word2hex(ZEROS_WORD, _littleEndian), WORD_SIZE);
		outBitstreamFile.write(word2hex(ZEROS_WORD, _littleEndian), WORD_SIZE);
	  }
	  else if (zerosCnt > 2){
		stringstream ss;
		ss << std::setfill ('0') << std::setw(WORD_SIZE*2) << std::hex << zerosCnt;
		outBitstreamFile.write(word2hex(ZEROS_CMD_WORD, _littleEndian), WORD_SIZE);
		outBitstreamFile.write(word2hex(ss.str(), _littleEndian), WORD_SIZE);
	  }
	  outBitstreamFile.write(buffer, WORD_SIZE);	  
	  zerosCnt = 0;
	} 
  }
  
  delete[] buffer;

  outBitstreamFile.close();
  inBitstreamFile.close();  
}


void
ZipData::unzipBitstream()
{
  unsigned int zerosCnt = 0;
  string wordLast = DUMMY_WORD;
  string wordCurrent = DUMMY_WORD;
  char *buffer = new char[WORD_SIZE]; 

  ifstream inBitstreamFile(_inputBitstream.c_str(), ios::in | ios::binary);
  ofstream outBitstreamFile(_outputBitstream.c_str(), ios::out | ios::binary);

  while (!inBitstreamFile.eof()){
	inBitstreamFile.read(buffer, WORD_SIZE);

	wordCurrent = hex2word(buffer, _littleEndian);

	if (wordCurrent == ZEROS_CMD_WORD){
	  wordLast = wordCurrent;	
	  continue;
	}
	
	if (wordLast == ZEROS_CMD_WORD){	  
	  zerosCnt = hex2uint(wordCurrent.c_str());
	  while (zerosCnt != 0){
	  	outBitstreamFile.write(word2hex(ZEROS_WORD, _littleEndian), WORD_SIZE);
	  	zerosCnt--;
	  }
	}
	else
	  outBitstreamFile.write(buffer, WORD_SIZE);

    if (wordLast == COMMAND_WORD && wordCurrent == DESYNC_WORD) break;
	wordLast = wordCurrent;	
  }
  
  delete[] buffer;

  outBitstreamFile.close();
  inBitstreamFile.close();  
}
