#ifndef CLEANHEADER_HH
#define CLEANHEADER_HH

#include <string>
#include <vector>


using namespace std;



class CleanHeader
{
public:
  CleanHeader(vector<string> inputBitstreams, string outputBitstream,
	      bool littleEndian){
    _inputBitstreams = inputBitstreams;
    _outputBitstream = outputBitstream;
    _littleEndian = littleEndian;
  };
  ~CleanHeader(){};
  void generateBitstream();
private:
  vector<string> _inputBitstreams;
  string _outputBitstream;
  bool _littleEndian;
  
  void writeHeader();
  void writeTail();
  void writeData(string inputBitstream, int seekHeader);
};


#endif
