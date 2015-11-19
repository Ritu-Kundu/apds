APDS : Approximate Patter-matching in Degenerate Strings
=========================================================

The code has the following modules :

Main Module:
-------------------------------------
It contains the main() method. Other than decoding the flags and reading the pattern and input files, it preprocesses them as follows:
- Preprocessing pattern-file:
  + Pattern file is assumed to be in the following format:
   * Each pattern starts with '> pattern' followed by an integer indicating pattern-number and an integer representing value of k(number of non-solid positions) in that pattern. 
   * Starting from the next line (until next '>' or end of file is hit), each row contains 1(=>present) or 0(=>absent) corresponding to each symbol/letter specified in the first line.
   * Assumed sequence of the letters of alphabet is- For DNA: "ACGT" and For Protein: "ACDEFGHIKLMNPQRSTVWY". 
   * A row represents a position/location in the pattern.
   * There can be no empty rows.
  + Following sanity checks must be performed:
   * There are exactly k non-solid positions in the pattern (neither less nor more than k).
   * k can not be greater than 235 as explained later.
  + Following information should be extracted:
   *	Matrix Degenerate[k, alphabetSize] such that Degenerate[i][j] = true => j^th letter is a valid character at i^th non-solid position. False represents the absence.
   * Array NonSolidPositions[k] such that NonSolidPositions[i]=j => i^th non-solid position in the pattern is at location j.
   *	Pattern sequence such that: 
    	- Each solid position is mapped to index of that character in the alphabet (For eg: G in DNA is 2). 
    	- Each non-solid position is replaced by a unique character calculated using the following formula: 
     	Since the longest alphabetsize(as of now) is 20, characters will be mapped from 0 to 19. => unique characters can take values from 255 to 21 (20 is used as delimitor for text-pattern combined sequence). We start from 254 and keep on dcreasing by 1 with each non-solid position. (that's why sanity check that k is less than 254-20 = 234+1.)
   * Length of the pattern sequence.

- Preprocessing input-file:
  + Input sequence is assumed to be a solid string.
  + Input file is assumed to be in the following format:
   * Each sequence starts with '>' followed by a string indicating sequence name (identifier).
   * 	Starting from the next line (until next '>' or end of file is hit), follows a sequence of characters containing letters of alphabet (For DNA: "ACGTN", For Protein: "ARNDCQEGHILKMFPSTWYV"). 
    	- New lines can be there between characters.
    	- letters can be either in upper or lower case.
  + Following information should be extracted:
   * 	Input sequence such that: 
    	- Each position is mapped to index of that character in the alphabet (For eg: G in DNA is 2). 

- Result is formatted as follows:
  + First line contains pattern numbers (to work as header for the following table).
  + After that follows a table with number of rows same as number of sequences. Each row represents a sequence. First column in each row of this table gives the sequence identifier. After that every column corresponds to a pattern. Element at row 'i' and column 'j' of this table gives the number of occurrences of j^th pattern in i^th sequence.
  + Next line after the table contains  '##### Summary ######' following which is given a summary of processing consisting of following:
   * The time used for calculation (including reading and writing the files).
   * The number of patterns and sequences used.
   * The number (and percentage) of sequences in which any of the pattern occurs at least once.

- One more detailed result file is generated with the same name as output file-name with an added extension of '.details'. For each sequence, it gives the identifier of the sequence followed by the location of each occurence of each pattern along with the substring of the sequence to which that particular pattern matches. It is formatted as sequence of sections for each text-sequence formatted as follows:
  + It starts '>' followed by the sequence identifier.
  + Following it, there is a subsection for each pattern. Each subsection begins with '#' followed by pattern id. 
    * After it each line contains an integer (indicating index of occurrence: 0-based) and a string (giving the matched substring of text) separated by a tab. 



### Organization:
- apds.cpp: main module
- globalDefs: contains the definitions, macros and declarations used by each of the other modules.


DegeneratePatternMatch Module:
---------------------------------------
Module implementing the main algorithm as a class.
- It provides the following funtions:
 * Returns the occurences of pattern(degenerate) in input-sequence.

- It prepares for constant-time LCA/LCP queries while being constructed.

- It is organised as follows:
 * DegeneratePatternMatch.cpp : 	implements the class.
 * DegeneratePatternMatch.hpp : 	defines the class.

Helper Module:
---------------------------------------
Module containing utility functions.
- It provides the following funtions:
 * A function to parse the input flags.
 * A function to measure time.
 * A function to print the usage of the tool.
 * a function to a given letter to its index in the corresponding alphabet. It makes use of two separate functions for mapping in DNA and PROT alphabet, respectively.

- It is organised as follows:
 * helper.cpp : 	contains the functions.
 * helperDefs.hpp : 	contains the declaration of functions and  macros.



NOTES
----------------------------------------
- Algorithm in paper is 1-based array, implementation is zero-based.
- Instead of the suffix tree as described in the paper, enhanced suffix array is used. RMQ on LCP array is used to answer longest
 common prefix queries.
 * For answering these queries, following libraries have been used:
  + [libdivsufsort](https://code.google.com/p/libdivsufsort/) 
  + [sdsl](https://github.com/simongog/sdsl-lite)





