
/** Module containing main() method which reads and preprocesses the input and pattern files in the form required for identification of the ocuurences.
 */

#include "globalDefs.hpp"
#include "helperDefs.hpp"
#include "DegeneratePatternMatch.hpp"

using namespace apds;

int main(int argc, char **argv){
  double startInitial = gettime();
  /* Decode arguments */
  struct InputFlags flags;
  if(decodeFlags (argc, argv, &flags ) == 0){
    usage();
    return (1);
  }

  /* Assign alphabet */
  std::string alphabet;
  int alphabetType;
  if (!strcmp("DNA", flags.alphabet)) {  
    alphabet = DNA;
    alphabetType = ALPHATYPEDNA;
  }
  else if (!strcmp("PROT", flags.alphabet)) {
    alphabet = PROT;
    alphabetType = ALPHATYPEPRO;
  }
  else {
    fprintf ( stderr, " Error: alphabet argument a should be `DNA' for nucleotide sequences or `PROT' for protein sequences!\n" );
    return ( 1 );
  }
  const int alphabetSize = (const int) alphabet.length();

  /* Read the pattern file in memory and preprocess it */
  FILE* pat_fd;  
  if(!(pat_fd = fopen(flags.pattern_filename, "r"))){
    fprintf (stderr,"Cannot open pattern file \n" );
    return (1);
  }
 

  std::vector<unsigned char*> pattern; // contains mapped pattern sequences
  std::vector<INT> patLen; // length of pattern sequence
  std::vector<int> K;
  std::vector<bool**>  degenerate;
  std::vector<INT *> nonSolidPositions;
  unsigned int maxAllocLen = 0;
  INT numPatterns = -1;

  INT row = 0;
  INT column = 0;
  INT kIndex = -1;
  INT symbolsAtCurrentPos = 0; // number of symbols found at current posiiton
  INT nonSolidPositionsCount = 0; // count of non-solid position
  char c;
  c = fgetc(pat_fd);
 
  while(c != EOF ) {
    if (c != '>') {
      fprintf ( stderr, " Error: pattern sequence %ld does not start with > !\n", (long) numPatterns+1 );
      return (1);
    }
    // next is pattern identifier(a num) and value of k
    std::vector<char> idLine;
    while ((c = fgetc(pat_fd)) != EOF && c != '\n' ) {
      idLine.push_back(c);
    }
    idLine.push_back('\0');
    std::string dummy1, dummy2; 
    int k;
    std::istringstream((char*) idLine.data()) >> dummy1 >> dummy2 >> k;
    numPatterns++;
    K.push_back(k);
    if(k > MAXUNIQUE){
      fprintf (stderr,"Pattern %ld : k > %d is not supported currently (unique characters are taken from range 254 to 20) \n",(long)numPatterns, MAXUNIQUE );
      return (1);
    }

    // A matrix[k, alphabetsize] denoting alphabet at degenerate positions
    // element at [i][j] is true if alphabet j may occur at ith non-solid position, false otherwise.
    degenerate.push_back(new bool*[k]);
    for(INT i =0; i<k; ++i){
      degenerate[numPatterns][i] = new bool[alphabetSize];
      std::fill_n(degenerate[numPatterns][i], alphabetSize, false);
    }

    // An array containing indices of non-solid positions
    // element at [i] = j => ith non-solid position is at j in pattern
    nonSolidPositions.push_back(new INT[k]);
    std::fill_n(nonSolidPositions[numPatterns], k, -1);

    // Read the input matrix (row=> position in pattern, column=> alphabet)
    row = 0;
    column = 0;
    kIndex = -1;
    symbolsAtCurrentPos = 0; // number of symbols found at current posiiton
    nonSolidPositionsCount = 0; // count of non-solid position
    unsigned char letter, firstLetter;

    pattern.push_back(NULL);
    maxAllocLen = 0;
    while ( (c = fgetc(pat_fd)) != EOF && c != '>' ){
      if (c == '1'){
	symbolsAtCurrentPos++;
	if (symbolsAtCurrentPos == 1) { // first symbol found for current pos
	  firstLetter = column; // save for later use
	  letter = column;
	}
	else if (symbolsAtCurrentPos == 2) { // two symbols found for this position
	  if (kIndex == k-1) { // non-solid positions exceed k
	    fprintf (stderr, " Error: pattern file contains more than %d non-solid position ( (K+1)th at row: %d)!\n", (int)k, (int)row );
	    return (1);
	  }

	  nonSolidPositions[numPatterns][++kIndex] = row; // it's a non-solid position
	  degenerate[numPatterns][kIndex][firstLetter] = true; // first letter
	  degenerate[numPatterns][kIndex][column] = true; //second letter
	  letter = UNIQUESTART - kIndex; //unique letter
	}
	else{ // more than 2 non-solid symbols
	  degenerate[numPatterns][kIndex][column] = true;
	}
	column++;
      }
      else if(c == '\n') { // this row ends
	// dynamic allocation of memory
	if ( row >= maxAllocLen ){
	  pattern[numPatterns] = (unsigned char*) realloc(pattern[numPatterns], (maxAllocLen+ALLOC_SIZE ) * sizeof(unsigned char));
	  maxAllocLen += ALLOC_SIZE;
	}
	pattern[numPatterns][row++] = letter;
	column=0;
	symbolsAtCurrentPos = 0;
      }
      else if (c == '0') {
	column++;
      }
    }
    patLen.push_back(row);
    // Another Sanity Check: No fewer than k non-solid positions
    if (kIndex < k-1) {
      fprintf (stderr, " Error: pattern %ld contains only %d non-solid symbols while it must contain exactly %d non-solid positions!\n",(long)numPatterns+1,  (int)kIndex+1, (int)k );
      return (1);
    }
  }
  numPatterns++;
  // close pattern file
  if(fclose(pat_fd)){
    fprintf (stderr,"File close error!\n");
    return (1);
  }

  /* Open result file and detailed result file */ 
  FILE* out_fd;
  if (!( out_fd=fopen(flags.output_filename, "w"))){
    fprintf (stderr," Error: Cannot open output file!\n");
    return ( 1 );
  }
  FILE* det_out_fd;
  std::string detFilename = std::string(flags.output_filename) + ".details";
  if (!( det_out_fd=fopen(detFilename.c_str(), "w"))){
    fprintf (stderr," Error: Cannot open detail output file!\n");
    return ( 1 );
  }
  // set up output file
  fprintf(out_fd, "\t\t\t");
  for (INT p=1; p <= numPatterns; ++p) {
    fprintf(out_fd, "%ld\t", (long) p);
  }
  fprintf(out_fd, "\n");

  /* Read the input file in memory */
  FILE* in_fd; 
  if(!(in_fd = fopen(flags.input_filename, "r"))){
    fprintf (stderr,"Cannot open input file \n" );
    return (1);
  }

  INT numSequences =0;
  unsigned char* sequence = NULL;
  INT seqLen = 0;
  INT numMatchedPatInSeq = 0;
  INT numPositiveSeq = 0;

  c = fgetc( in_fd );
  while(c != EOF ){
    if (c != '>') {
      fprintf ( stderr, " Error: input sequence %ld does not start with > !\n", (long) (numSequences+1) );
      return (1);
    }

    // next is sequence identifier
    std::vector<char> idLine;
    while ( ( c = fgetc( in_fd ) ) != EOF && c != '\n' ) {
      idLine.push_back(c);
    }
    idLine.push_back('\0');
    // start reading sequence
    sequence = NULL;
    seqLen = 0;
    maxAllocLen = 0;
    numMatchedPatInSeq = 0;
    while((c = fgetc(in_fd)) != EOF && c != '>'){
      if(c == '\n' || c == '\r'){continue;} // ignore new lines
      c = toupper(c);
      // dynamic allocation of memory
      if ( seqLen >= maxAllocLen ){
	sequence = (unsigned char*) realloc(sequence, (maxAllocLen+ALLOC_SIZE ) * sizeof(unsigned char));
	maxAllocLen += ALLOC_SIZE;
      }
      if(alphabet.find(c) != std::string::npos){
	sequence[seqLen++] = mapping(alphabetType, c);
      }
      else{
	fprintf (stderr, " Error: input file contains an unexpected character %c in sequence %s at pos:%ld!\n", c, idLine.data(), (long)seqLen );
	return (1);
      }
    }

    fprintf(out_fd, "%s \t", idLine.data() );
    fprintf(det_out_fd, "> %s \n", idLine.data() );
    /* Check occurrences of each pattern in this sequence */
    numMatchedPatInSeq = 0;
    for (INT p=0; p  < numPatterns; ++p) {
      std::list<INT> occurences;
      // Find occurences 
      DegeneratePatternMatch dpm(sequence, seqLen, pattern[p], patLen[p], K[p], degenerate[p], nonSolidPositions[p]); 
      dpm.getApproxPatternOccurence(occurences);

      /* Write output */
      fprintf(out_fd, "%ld \t", (long) occurences.size());
      fprintf(det_out_fd, "#%ld \n", (long) p+1);
      std::list<INT>::iterator it;
      INT occ=-1;
      for (it = occurences.begin(); it != occurences.end(); ++it){
	occ = *it;
	fprintf(det_out_fd, "%ld\t", (long)occ);
	for (int i = 0; i < patLen[p]; ++i){
	  fprintf(det_out_fd, "%c", revMapping(alphabetType, sequence[occ+i]));      
	}
	fprintf(det_out_fd, "\n");
      }

      /* Analysis */
      if (occurences.size() >= 1){
	++numMatchedPatInSeq;
      }
    }
    fprintf(out_fd, "\n");
    // Free mem
    free(sequence);
    ++numSequences;
    /* Analysis */
    if (numMatchedPatInSeq >= 1) {
       ++numPositiveSeq;
    }

  }

  double endFinal = gettime();
  fprintf(out_fd, "##### Summary ######\n\n");
 
  fprintf(out_fd, "Total Elapsed time for processing: %lf secs.\n\n", (endFinal - startInitial) );
  fprintf(out_fd, "Total number of patterns: %ld \n", (long) numPatterns );
  fprintf(out_fd, "Total number of sequences: %ld \n", (long) numSequences );
  fprintf(out_fd, "Number of positive sequences found: %ld (%.2f percent) \n" , (long) numPositiveSeq, (numPositiveSeq*100.0/numSequences) ); 
 

  // close files
  if(fclose(in_fd)){
    fprintf (stderr,"File close error in input file!\n");
    return (1);
  }
  if(fclose(out_fd)){
    fprintf (stderr,"File close error in output file!\n");
    return (1);
  }

  /* clean up */
  delete[] flags.alphabet;
  delete[] flags.pattern_filename;
  delete[] flags.input_filename;
  delete[] flags.output_filename;
  for (INT p=0; p  < numPatterns; ++p) {
    for(INT i =0; i<K[p]; ++i) {
      delete[] degenerate[p][i];
    }
    delete[] degenerate[p];
    delete[] nonSolidPositions[p];
    free(pattern[p]);
  }

}
