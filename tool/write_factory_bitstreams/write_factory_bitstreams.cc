#include <iostream>
#include <string>

#include <CleanHeader.hh>
#include <ZipData.hh>


#define VERSION "1.0.0"

#define SUCCESS 0
#define FAIL 1

using namespace std;

vector <string> _inputBitstreams;
string _outputBitstream;
bool _version;
bool _littleEndian;
bool _zipBitstreams;
bool _unzipBitstream;


void
usage(const char* app)
{
  cerr << endl << "Usage: " << app << " [options] <input bitstream 1> <input bitstream 2> "
       << "<input bitstream 3> ... <output bitstream>\n\n" 
       << "\tOptions:\n" << endl
       << "\t\t-h, --help            Shows this message\n"<< endl 
       << "\t\t-le, --little-endian  Little-Endian mode (Default: Big-Endian)\n"<< endl 
       << "\t\t-z, --zip             Zip bitstream(s)\n"<< endl 
       << "\t\t-e, --unzip           Unzip bitstream\n"<< endl 
       << "\t\t-v, --version         Display the tool version\n"<< endl << endl
       << "\t For dummy bitstream (Header and Desync word) you have to introduce a blanking file as input bitstream\n" << endl << endl;
}


void
parseArgs(const int argc, char* argv[])
{
  int i;
  string arg;
  _version = false;
  _zipBitstreams = false;
  _unzipBitstream = false;
  _littleEndian = false;

  for(i = 1; i != argc; i++){
    arg = argv[i];

    if (arg == "-v" || arg == "--version"){
      _version = true;
      return;
    }
    if (arg == "-z" || arg == "--zip"){
      _zipBitstreams = true;
      continue;
    }
    if (arg == "-e" || arg == "--unzip"){
      _unzipBitstream = true;
      continue;
    }
    if (arg == "-le" || arg == "--little-endian"){
      _littleEndian = true;
      continue;
    }
    if (i == argc-1)
      _outputBitstream = argv[argc-1];
    else
      _inputBitstreams.push_back(argv[i]);
  }
}


int
checkBitstreams()
{
  vector<string>::iterator itBitstreams;

  for(itBitstreams = _inputBitstreams.begin();
      itBitstreams != _inputBitstreams.end(); itBitstreams++)
    if(_outputBitstream == (string)*itBitstreams){
      cout << "One input file and output file should be different" << endl;
      return FAIL;
    }

  return SUCCESS;
}


int
checkOptions(int argc, char* argv[])
{
  if (argc < 3){
    cout << "Options wrong" << endl;    
    return FAIL;
  }

  if(_unzipBitstream && _zipBitstreams){
    cout << "You have to use one of these options: unzip or zip" << endl; 
    return FAIL;
  }

  if(_unzipBitstream){
    if ((argc == 5 && !_littleEndian) || argc > 5){
      cout << "Unzip usage: " << argv[0] << " [-le] -e <input.bit> <output.bit>" << endl;
      return FAIL;
    }
  }  

  return SUCCESS;
}


int
check(int argc, char* argv[])
{
  if (checkBitstreams() == FAIL) return FAIL;
  if (checkOptions(argc, argv) == FAIL) return FAIL;  

  return SUCCESS;
}


int
process()
{
  // Unzip section
  if (_unzipBitstream){
    ZipData *zipData = new ZipData(_inputBitstreams[0], _outputBitstream, _littleEndian);
    zipData->unzipBitstream();

    delete zipData;
    
    return SUCCESS;  
  }
  // --------------------------------------------------------------------------


  string outputFile = _outputBitstream;
  if (_zipBitstreams) outputFile = _outputBitstream + "_full";

  CleanHeader *cleanHeader = new CleanHeader(_inputBitstreams, outputFile, _littleEndian);
  cleanHeader->generateBitstream();

  // Zip section
  if (_zipBitstreams){
    ZipData *zipData = new ZipData(outputFile, _outputBitstream, _littleEndian);
    zipData->zipBitstream();
    delete zipData;
  }
  // --------------------------------------------------------------------------
  

  delete cleanHeader;

  return SUCCESS;  
}


int
main(int argc, char* argv[])
{  
  parseArgs(argc, argv);

  if (_version){
    cout << argv[0] << " version: " << VERSION << endl;
    return SUCCESS;
  }

  if (check(argc, argv)){
    usage(argv[0]);
    return FAIL;
  }

  return process();  
}

