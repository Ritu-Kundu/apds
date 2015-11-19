/** Module implementing the class DegeneratePatternMatch
 */
#ifndef DEGENERATEPATTERNMATCH_CPP
#define DEGENERATEPATTERNMATCH_CPP

#include "DegeneratePatternMatch.hpp"
#include "helperDefs.hpp"

namespace apds{
  DegeneratePatternMatch::DegeneratePatternMatch(unsigned char* s, const INT sl, unsigned char* p, const INT pl, const INT k, bool** d, INT* nsp):k(k), sequence(s), sLen(sl), pattern(p), pLen(pl), degenerate(d), nonSolidPositions(nsp){
    /* Prepare for LCP queries */
    prepareForRMQ();
  }

  DegeneratePatternMatch::~DegeneratePatternMatch(void){
    delete[] rank;
    delete[] lcp;
    delete rmq;
  }

  void
  DegeneratePatternMatch::getApproxPatternOccurence(std::list<INT>& result){

    if (pLen > sLen) {
      fprintf (stderr, " Error: pattern longer than text!\n");
      return;
    }
  
    /* Prepare Matrix[n-m, k] with sentinel col */
    INT rows = sLen-pLen+1;
    INT columns = k + 1;
    INT defaultVal = pLen;
    INT** mismatch = new INT*[rows];
    for(INT i =0; i<rows; ++i){
      mismatch[i] = new INT[columns];
      std::fill_n(mismatch[i], columns, defaultVal);
    }

    /**
       Stage 2
    */
    std::list<INT> approximateMatch;
    /* Calculate mismatch and approximateMatch */
    INT f =0;
    for (INT i=0; i < rows; ++i) {
      f = 0;
      for (INT j=0; j < columns; ++j) {
	mismatch[i][j] = f + getLongestCommonPrefix(i+f, sLen+f);
	f = mismatch[i][j]+1;
      }
      if (mismatch[i][k] == defaultVal) {
	approximateMatch.push_back(i);
      }
    }

    /**
       Stage 3
    */
    std::list<INT>::iterator it;
    INT testPos, nonSolidPos;
    bool flagAllFake = true;
    int sequenceLetter, patternLetter;
    for (it = approximateMatch.begin(); it != approximateMatch.end(); ++it) {
      testPos = *it;
      flagAllFake = true;
      for (INT j = 0; j < k; ++j) {
	nonSolidPos = nonSolidPositions[j];
	sequenceLetter = sequence[testPos + nonSolidPos];
	if (!degenerate[j][sequenceLetter]) { // real mismatch
	  flagAllFake = false;
	  break;
	}
      }
      if (flagAllFake) { // all fake mismatches
	result.push_back(testPos);
      }
    }
    // clean-up
    for(INT i =0; i<rows; ++i){
      delete[] mismatch[i];
    }
    delete[] mismatch;
  }

  unsigned int
  DegeneratePatternMatch::prepareForRMQ(){
    /* Prepare SP$  */
    const INT len = (sLen + pLen) + 1;
    unsigned char* text = new unsigned char[len];
    std::copy(sequence,sequence+sLen ,text );
    std::copy(pattern,pattern+pLen ,text+sLen );
    text[len-1] = DELIM;

    /* Compute Suffix Array */
    INT* sa = new INT[len];
    if(sa == NULL){
      fprintf (stderr,"Cannot allocate memory for SA.");
      return (0);
    }
#ifdef _USE_64
    if(divsufsort64(text, sa, len) != 0){
      fprintf (stderr, "SA computation failed");
      return (0);
    }
#endif
#ifdef _USE_32
    if(divsufsort(text, sa, len) != 0){
      fprintf (stderr,"SA computation failed");
      return (0);
    }
#endif
    /* Compute Rank array */ 
    rank = new INT[len];
    if(rank == NULL){
      fprintf (stderr,"Cannot allocate memory for Rank Array");
      return (0);  
    }
    for(INT i = 0; i < len; i++ ){
      rank [sa[i]] = i;
    }

    /* Compute LCP array */ 
    lcp = new INT[len];
    if(lcp == NULL){
      fprintf (stderr,"Cannot allocate memory for LCP Array"); 
      return (0); 
    }       
    if(constructLCPArray(text, len, sa) != 1){
      fprintf (stderr,"LCP computation failed");
      return (0);
    }

    // Delete SA and text as they are not needed now
    delete[] sa;
    delete[] text;

    /* Prepare LCP array for RMQ */ 
    // create a vector of length len and initialize it with 0s
    sdsl::int_vector<> v(len , 0 ); 
    for(INT i = 0; i < len; i++){
      v[i] = lcp[i];
    }
    rmq = new sdsl::rmq_succinct_sct<>(&v);
    // v is not required now
    sdsl::util::clear(v);  
    //cout << "prepared for rmq" << endl;
  }

  unsigned int 
  DegeneratePatternMatch::constructLCPArray( unsigned char* text, INT len, INT* sa)
  {
    INT l = 0;
    INT lowest = 0;
    INT i, j2;
    for(int j = 0; j < len; ++j){
      l = std::max(lowest, l-1);  
      i = rank[j];
      if(i != 0){
	j2 = sa[i-1];
	while((j+l < len) && (j2+l < len) && (text[j+l] == text[j2+l])){
	  l++;
	}
      }
      else{
	l = 0;
      }
      lcp[i] = l;
    }
    return 1;
  }

  INT
  DegeneratePatternMatch::getLongestCommonPrefix(INT l, INT r){
    INT l_rmq, r_rmq;
    INT rx = rank[l];
    INT ry = rank[r];
    if(rx < ry){l_rmq=rx; r_rmq=ry;}
    else{l_rmq=ry; r_rmq=rx;}
    return lcp[(*rmq)(l_rmq+1, r_rmq)];
  }

}// end name space

#endif



