/** Header file containing the definitions and declarations used by helper module.
 */

#ifndef HELPER_DEFS
#define HELPER_DEFS

#include <sys/time.h>
#include <getopt.h>

#include "globalDefs.hpp"
namespace apds{
  struct InputFlags{
    char* alphabet;
    char* pattern_filename;
    char* input_filename;
    char* output_filename;

  };

  static struct option long_options[] =
    {
      { "alphabet",                required_argument, NULL, 'a' },
      { "pattern-file",            required_argument, NULL, 'p' },
      { "input-file",              required_argument, NULL, 'i' },
      { "output-file",             required_argument, NULL, 'o' },
      { "help",                    no_argument,       NULL, 'h' },
      { NULL,                      0,                 NULL, 0   }
    };

  /** Usage of the tool 
   */  
void usage (void);

  /** Decodes the input flags 
   */
  int decodeFlags(int argc, char* argv [], struct InputFlags* flags);
  
  /** Measures time
   */
  double gettime(void);

  /** Maps a given letter to its index in corresponding alphabet
   * @param alphabetType ALPHATYPEDNA for DNA, ALPHATYPEPRO for protein
   * @param b input character
   * @return index of b in alphabet
   */
  int mapping (int alphabetType, unsigned char b);

  /** Maps given character in DNA alphabet 
   */
  int mappingDNA (unsigned char b);

 /** Maps given character in PROT alphabet 
   */
  int mappingProtein (unsigned char b);

  /** Maps a given index to the letter in corresponding alphabet
   * @param alphabetType ALPHATYPEDNA for DNA, ALPHATYPEPRO for protein
   * @param b input index
   * @return character at b in alphabet
   */
  unsigned char revMapping (int alphabetType, INT b);

  /** Maps given index in DNA alphabet to character 
   */
  unsigned char revMappingDNA (INT b);

  /** Maps given character in PROT alphabet 
   */
  unsigned char revMappingProtein (INT b);
} // end namespace

#endif
