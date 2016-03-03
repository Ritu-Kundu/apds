/**
    APDS : Approximate Patter-matching in Degenerate Strings
    Copyright (C) 2016 Ritu Kundu, Fatima Vayani, Manal Mohamed 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

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
