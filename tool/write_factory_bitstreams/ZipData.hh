#ifndef ZIPDATA_HH
#define ZIPDATA_HH


#include <iostream>
#include <string>

using namespace std;


class ZipData
{
public:
  ZipData(string inputBitstream, string outputBitstream,
	  bool littleEndian){
    _inputBitstream = inputBitstream;
    _outputBitstream = outputBitstream;
    _littleEndian = littleEndian;
  };
  ~ZipData(){};
  void zipBitstream();
  void unzipBitstream();
private:
  string _inputBitstream;
  string _outputBitstream;
  bool _littleEndian;
};


#endif
