/** Module containing utility functions.
 */
#include "helperDefs.hpp"

namespace apds{

  int 
  decodeFlags(int argc, char* argv [], struct InputFlags* flags){
    int oi;
    int opt;
    int val;
    char* ep;
    int args=0;  
    /* initialisation */
    flags -> alphabet = NULL;
    flags -> pattern_filename = NULL;
    flags -> input_filename = NULL;
    flags -> output_filename = NULL;
    while ( ( opt = getopt_long(argc, argv, "t:c:a:p:i:o:h", long_options, &oi)) != - 1){
      switch (opt){
      case 'a':
	{
	  std::string alph(optarg);
	  flags->alphabet = new char[alph.size() + 1];
	  alph.copy(flags->alphabet, alph.size());
	  flags->alphabet[alph.length()] = '\0';
	  args ++;
	  break;
	}

      case 'p':
	{
	  std::string patFile(optarg);
	  flags->pattern_filename = new char[patFile.length() + 1];
	  patFile.copy(flags->pattern_filename, patFile.length());
          flags->pattern_filename[patFile.length()] = '\0';
	  args ++;
	  break;
	}

      case 'i':
	{
	  std::string inFile(optarg);
	  flags->input_filename = new char[inFile.length() + 1];
	  inFile.copy(flags->input_filename, inFile.length());
          flags->input_filename[inFile.length()] = '\0';
	  args ++;
	  break;
	}

      case 'o':
	{
	  std::string outFile(optarg);
	  flags->output_filename = new char[outFile.length() + 1];
	  outFile.copy(flags->output_filename, outFile.length());
          flags->output_filename[outFile.length()] = '\0';

	  args ++;
	  break;
	}

      case 'h':
	return (0);
      }
    }
    if ( args < 4 ){
      return (0);
    }
    else{
      return (optind);
    }
  }

  void 
  usage(void){
    fprintf ( stdout, " Usage: apds <options>\n" );
    fprintf ( stdout, " Standard (Mandatory):\n" );
    fprintf ( stdout, "  -a, --alphabet \t \t <str>     `DNA' for nucleotide  sequences or `PROT' for protein  sequences. \n" );
    fprintf ( stdout, "  -p, --pattern-file \t \t  <str>     Input file  name.\n" );
    fprintf ( stdout, "  -i, --input-file \t \t  <str>     Input file  name.\n" );
    fprintf ( stdout, "  -o, --output-file \t \t  <str>     Output filename.\n" );
 
  }

  double 
  gettime(void){
    struct timeval ttime;
    gettimeofday( &ttime , 0 );
    return ttime.tv_sec + ttime.tv_usec * 0.000001;
  }

  int
  mapping (int alphabetType, unsigned char b){
    int a = -1;
    switch (alphabetType) {
 
    case ALPHATYPEDNA:
      a = mappingDNA(b);
      break;
    case ALPHATYPEPRO:
      a = mappingProtein(b);
      break;
    }
    return (a);
  }

  int 
  mappingDNA (unsigned char b){
    int a = -1;
    switch (b) {
    case 'A':
      a = 0;
      break;
    case 'C':
      a = 1;
      break;
    case 'G':
      a = 2;
      break;
    case 'T':
      a = 3;
      break;
    }
    return (a);
  }

  int 
  mappingProtein (unsigned char b){
    int a = -1;
    switch (b) {
    case 'A':
      a = 0;
      break;
    case 'C':
      a = 1;
      break;
    case 'D':
      a = 2;
      break;
    case 'E':
      a = 3;
      break;
    case 'F':
      a = 4;
      break;
    case 'G':
      a = 5;
      break;
    case 'H':
      a = 6;
      break;
    case 'I':
      a = 7;
      break;
    case 'K':
      a = 8;
      break;
    case 'L':
      a = 9;
      break;
    case 'M':
      a = 10;
      break;
    case 'N':
      a = 11;
      break;
    case 'P':
      a = 12;
      break;
    case 'Q':
      a = 13;
      break;
    case 'R':
      a = 14;
      break;
    case 'S':
      a = 15;
      break;
    case 'T':
      a = 16;
      break;
    case 'V':
      a = 17;
      break;
    case 'W':
      a = 18;
      break;
    case 'Y':
      a = 19;
      break;
    }
    return ( a );
  }

  unsigned char
  revMapping (int alphabetType, INT b){
    unsigned char a = 255;
    switch (alphabetType) {
 
    case ALPHATYPEDNA:
      a = revMappingDNA(b);
      break;
    case ALPHATYPEPRO:
      a = revMappingProtein(b);
      break;
    }
    return (a);
  }

  unsigned char 
  revMappingDNA ( INT b){
    unsigned char a = 255;
    switch (b) {
    case 0:
      a = 'A';
      break;
    case 1:
      a = 'C';
      break;
    case 2:
      a = 'G';
      break;
    case 3:
      a = 'T';
      break;
    }
    return (a);
  }

  unsigned char 
  revMappingProtein ( INT b){
    unsigned char a = 255;
    switch (b) {
    case 0:
      a = 'A';
      break;
    case 1:
      a = 'C';
      break;
    case 2:
      a = 'D';
      break;
    case 3:
      a = 'E';
      break;
    case 4:
      a = 'F';
      break;
    case 5:
      a = 'G';
      break;
    case 6:
      a = 'H';
      break;
    case 7:
      a = 'I';
      break;
    case 8:
      a = 'K';
      break;
    case 9:
      a = 'L';
      break;
    case 10:
      a = 'M';
      break;
    case 11:
      a = 'N';
      break;
    case 12:
      a = 'P';
      break;
    case 13:
      a = 'Q';
      break;
    case 14:
      a = 'R';
      break;
    case 15:
      a = 'S';
      break;
    case 16:
      a = 'T';
      break;
    case 17:
      a = 'V';
      break;
    case 18:
      a = 'W';
      break;
    case 19:
      a = 'Y';
      break;
    }
    return ( a );
  }
} // end namespace
