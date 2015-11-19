/** Module defining the class that implements the main algorithm
 */
#ifndef DEGENERATEPATTERNMATCH_HPP
#define DEGENERATEPATTERNMATCH_HPP

#include "globalDefs.hpp"

// include appropriate header for suffix sort
#ifdef _USE_64
#include <divsufsort64.h>
#endif

#ifdef _USE_32
#include <divsufsort.h>
#endif

#include <sdsl/rmq_support.hpp>	

namespace apds{
  /** Class DegeneratePatternMatch
   * This class provides for the following:
   *  - Returns the occurences of pattern(degenerate) in input-sequence.
   * It prepares for constant-time LCA/LCP queries while being constructed.
   */


  class DegeneratePatternMatch{
  public:
    /** Constructor which also prepares for constant-time LCP queries.
     * @param s pointer to array containing input sequence
     * @param sl length of input-sequence
     * @param p pattern pointer to array containing pattern-sequence(after substituting unique symbols at non-solid positions)
     * @param pl pattern-length
     * @param k number of non-solid positions
     * @param d pointer to boolean matrix[k, alphabetsize] denoting alphabet at degenerate positions.
     * @param nsp pointer to array containing indices of non-solid positions.
     */
    DegeneratePatternMatch(unsigned char* s, const INT sl, unsigned char* p, const INT pl, const INT k, bool** d, INT* nsp);  

    /** Destructor
     */
    ~DegeneratePatternMatch();

    /** Returns the indices of occurences in the given list */
    void getApproxPatternOccurence(std::list<INT>& result);


  private:
    /** Rank array of text-pattern combined sequence */
    INT* rank;

    /** LCP array of text-pattern combined sequence */
    INT* lcp;

    /** RMQ structure to answer constant-time rmq query */
    sdsl::rmq_succinct_sct<>* rmq;

    /** Number of non-solid positions */
    const INT k;

    /** Length of input-sequence */
    const INT sLen;

    /** Length of substituted degenerate pattern */
    const INT pLen;

    /** Pointer to substituted-pattern sequence array */
    unsigned char* pattern;

    /** Pointer to input-sequence array */
    unsigned char* sequence;

    /** Pointer to boolean matrix[k, alphabetsize] 
     *  It enoting alphabet at degenerate positions such that
     d[i][j] is true if alphabet j occurs at ith non-solid position, false otherwise.
    */
    bool** degenerate;

    /** Pointer to array[k]
     *  that contains indices of non-solid positions such that
     nsp[i] = j => ith non-solid position is at j in pattern 
    */
    INT* nonSolidPositions;

    /** Preapres for constant time RMQ queries */
    unsigned int prepareForRMQ();

    /** Calculates LCP array 
     * @param text pointer to text whose LCP is to be calculated
     * @param len length of this given sequence
     * @param sa pointer to suffix array of the text
     * @returns 1 
     */
    unsigned int constructLCPArray(unsigned char* text, 
				   INT len, INT* sa);

    /** Returns Longest Common Prefix of suffices starting at given l and r */
    INT getLongestCommonPrefix(INT l, INT r);
  
  };

}// end name space

#endif



