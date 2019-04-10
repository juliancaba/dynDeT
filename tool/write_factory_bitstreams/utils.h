#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>

using namespace std;

#define SYNC_WORD         "aa995566"
#define SYNC_WORD_LE      "665599aa"
#define DUMMY_WORD        "ffffffff"
#define WIDTH_AUTO_WORD   "000000bb"
#define CONFIG_WORD       "11220044"
#define DESYNC_WORD       "0000000d"
#define COMMAND_WORD      "30008001"
#define NOOP_WORD         "20000000"
#define FDRI_WORD         "30004000"
#define FDRI_WORD_LE      "00400030"
#define ZEROS_CMD_WORD    "30036001"
#define ZEROS_CMD_WORD_LE "01600330"
#define ZEROS_WORD        "00000000"

#define WORD_SIZE      4

char *word2hex(string word, bool littleEndian);
string hex2word(char *buffer, bool littleEndian);
void SLR(string *word, string byte);
int getSeekEndHeader(string bitstream, bool littleEndian);
unsigned int hex2uint(const char *chex);

#endif
