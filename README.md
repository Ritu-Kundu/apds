APDS : Approximate Patter-matching in Degenerate Strings
=====================================

APDS is a tool that finds out approximate occurences of a degenerate pattern in a given input sequence.
It is based on the algorithm described in the paper titled
[**Linear Algorithm for Conservative Degenerate Pattern**](http://www.sciencedirect.com/science/article/pii/S0031320301001790)
by Maxime Crochemore, Costas S. Iliopoulos, Ritu Kundu, Manal Mohamed, Fatima Vayani

To compile APDS, please follow the instructions given in file INSTALL.

To have an overview of the code-organisation, please read DevelopersNotes.md.

**Usage of the tool:** 
apds <options>
with the following options:
- -a, --alphabet	<str>	"DNA"(for DNA) or "PROT"(for Protein sequence)
- -p, --pattern-file	<str>	Pattern file  name.
- -i, --input-file	<str>	Input file  name.
- -o, --output-file	<str>	Output filename.

 Example:  ./apds  -k 2 -a PROT -p ./data/pattern.txt -i ./data/sample.txt -o ./result/sampleOutput.txt

Here, the sequence is given in file "sample.txt" which is in subfolder "data" of current folder.
The patter is in file "pattern.txt" (in subfolder "data" of current folder). 
Occurences of the degenerate pattern will be identified in te input sequence and
result will be written in the file "realOutput.txt" in subfolder "result" of current folder.

*** Notes ***
- K should not be greater than 235.
- Pattern file is assumed to be in the following format:
 * Each pattern starts with '> pattern' followed by an integer indicating pattern-number and an integer representing value of k(number of non-solid positions) in that pattern. 
 * Starting from the next line (until next '>' or end of file is hit), each row contains 1(=>present) or 0(=>absent) corresponding to each symbol/letter specified in the first line.
 * Assumed sequence of the letters of alphabet is- For DNA: "ACGT" and For Protein: "ACDEFGHIKLMNPQRSTVWY".
 * Starting from second line, each row contains 1(=>present) or 0(=>absent) corresponding to each symbol/letter specified in the first line.
 * A row represents a position/location in the pattern.
 * There can be NO empty rows.
 * There should exactly be k non-solid positions.

- Input file is assumed to be in the following format:
  + Input sequence is assumed to be a solid string.
  + Input file is assumed to be in the following format:
   * Each sequence starts with '>' followed by a string indicating sequence name (identifier).
   * Starting from the next line (until next '>' or end of file is hit), follows a sequence of characters containing letters of alphabet (For DNA: "ACGTN", For Protein: "ARNDCQEGHILKMFPSTWYV").
    + New lines can be there between characters. 
    + Letters can be either in upper or lower case.
 * There can be empty rows.

- Output file is in the following format:
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


- Instead of the suffix tree as described in the paper, enhanced suffix array is used. RMQ on LCP array is used to answer longest
 common prefix queries.
 * For answering these queries, following libraries have been used:
  + [libdivsufsort](https://code.google.com/p/libdivsufsort/) 
  + [sdsl](https://github.com/simongog/sdsl-lite)

- Solon's [MAW](https://github.com/solonas13/maw) helped as an inspiration for better organization of the code.
