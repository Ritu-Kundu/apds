/** Header file containing the definitions, macros and declarations used by each of the other modules.
 */
#ifndef APDS_DEFS
#define APDS_DEFS

#include <stdio.h>
#include <string>
#include <list>
#include <iterator>
#include <sstream>  
#include <vector>


#define ALLOC_SIZE 1048576
#define ALPHATYPEDNA 0
#define ALPHATYPEPRO 1
#define DNA "ACGT" //DNA alphabet
#define PROT "ACDEFGHIKLMNPQRSTVWY"  //Proteins alphabet
#define MAXUNIQUE  235  // 255-20 = 235
#define DELIM 20
#define UNIQUESTART 254
#define UNIQUEEND 91


namespace apds{
#ifdef _USE_64
typedef int64_t INT;
#endif

#ifdef _USE_32
typedef int32_t INT;
#endif

} // end namespace
#endif
