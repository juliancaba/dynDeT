#include <stdlib.h>
#include <fstream>

#include <utils.h>
#include <CleanHeader.hh>


void
CleanHeader::generateBitstream()
{
  vector<string>::iterator itBitstreams;


  writeHeader();
  
  for(itBitstreams = _inputBitstreams.begin();
      itBitstreams != _inputBitstreams.end(); itBitstreams++)
    writeData((string)*itBitstreams, getSeekEndHeader((string)*itBitstreams, _littleEndian));

  writeTail();
}


void
CleanHeader::writeHeader()
{
  ofstream outBitstreamFile(_outputBitstream.c_str(), ios::out | ios::binary);
  

  outBitstreamFile.write(word2hex(DUMMY_WORD, _littleEndian), WORD_SIZE);
  outBitstreamFile.write(word2hex(DUMMY_WORD, _littleEndian), WORD_SIZE);
  outBitstreamFile.write(word2hex(WIDTH_AUTO_WORD, _littleEndian), WORD_SIZE);
  outBitstreamFile.write(word2hex(CONFIG_WORD, _littleEndian), WORD_SIZE);
  outBitstreamFile.write(word2hex(DUMMY_WORD, _littleEndian), WORD_SIZE);
  outBitstreamFile.write(word2hex(DUMMY_WORD, _littleEndian), WORD_SIZE);
  outBitstreamFile.write(word2hex(SYNC_WORD, _littleEndian), WORD_SIZE);
  
  outBitstreamFile.close();
}


void
CleanHeader::writeTail()
{
  ofstream outBitstreamFile(_outputBitstream.c_str(), ios::out | ios::binary | ios::app);
  
  outBitstreamFile.write(word2hex(COMMAND_WORD, _littleEndian), WORD_SIZE);
  outBitstreamFile.write(word2hex(DESYNC_WORD, _littleEndian), WORD_SIZE);
  outBitstreamFile.write(word2hex(DESYNC_WORD, _littleEndian), WORD_SIZE);
  
  outBitstreamFile.close();
}


void
CleanHeader::writeData(string inputBitstream, int seekHeader)
{
  //int seek = 0;
  //  long size;
  string wordLast = DUMMY_WORD;
  string wordCurrent = DUMMY_WORD;
  char *buffer = new char[WORD_SIZE]; 


  ifstream inBitstreamFile(inputBitstream.c_str(), ios::in | ios::binary);
  ofstream outBitstreamFile(_outputBitstream.c_str(), ios::out | ios::binary | ios::app);

  inBitstreamFile.seekg(seekHeader);


  while (!inBitstreamFile.eof()){
    inBitstreamFile.read(buffer, WORD_SIZE);

    wordCurrent = hex2word(buffer, _littleEndian);
      

    if (wordLast == COMMAND_WORD && wordCurrent == DESYNC_WORD) break;
    else if (wordLast == COMMAND_WORD)
    	outBitstreamFile.write(word2hex(COMMAND_WORD, _littleEndian), WORD_SIZE);

    wordLast = wordCurrent;
    if (wordCurrent == COMMAND_WORD) continue;

    outBitstreamFile.write(buffer, WORD_SIZE);	
  }
  
  delete[] buffer;

  outBitstreamFile.close();
  inBitstreamFile.close();
}

