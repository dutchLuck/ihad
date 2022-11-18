/*
 * B Y T E F R E Q . C
 *
 * Index Hex Ascii Dump of a (binary) file or stdin.
 *
 * byteFreq.c last edited on Fri Nov 18 23:32:10 2022 
 *
 * This is not production code! Consider it only slightly tested.
 *
 * byteFreq was written for my own education.
 *
 * This code is released under the MIT license
 *
 */

/*
 * Output a listing of byte freq from an array.
 * 
 */

/*
 * $Log: byteFreq.c,v $
 * Revision 0.1  2022/11/18 12:32:43  owen
 * Added % frequency into Frequency: line for -v4 and -v5 options
 *
 * Revision 0.0  2022/11/16 12:24:45  owen
 * Print each byte frequency on a separate line.
 *
 *
 */

#include <stdio.h>	/* printf() fprintf() */
#include <limits.h> /* LONG_MIN LONG_MAX */

#define  SRC_CODE_CNTRL_ID  "$Id: byteFreq.c,v 0.1 2022/11/18 12:32:43 owen Exp owen $"

#define  BYTE_MASK 0xff

char *  humanReadableASCII[] = {
 "NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL", "BS", "HT", "LF", "VT", "FF", "CR", "SO", "SI",
 "DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYN", "ETB", "CAN", "EM", "SUB", "ESC", "FS", "GS", "RS", "US", 
 "SP", "!", "\"", "#", "$", "%", "&", "'", "(", ")", "*", "+", ",", "-", ".", "/",
 "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ":", ";", "<", "=", ">", "?",
 "@", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O",
 "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "[", "\\", "]", "^", "_",
 "`", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o",
 "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "{", "|", "}", "~", "DEL",
 " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ",
 " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ",
 " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ",
 " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ",
 " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ",
 " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ",
 " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ",
 " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "
};


int  doByteFreqStats( long  byteFreq[], long long *  totalCount, int *  indxOfMax, int *  indxOfMin )  {
  int  byteIndx;
  int  zeroFreqCount = 0;
  long long  total = 0LL;
  int  indxOfCurrentMax = 0;
  int  indxOfCurrentMin = 0;
  long  valueOfCurrentMax;
  long  valueOfCurrentMin;
  
  valueOfCurrentMax = LONG_MIN;
  valueOfCurrentMin = LONG_MAX;
  for( byteIndx = 0; byteIndx < 256; byteIndx++ )  {
    total += (long long) byteFreq[ byteIndx ];
    if( byteFreq[ byteIndx ] > valueOfCurrentMax )  {
      valueOfCurrentMax = byteFreq[ byteIndx ];
      indxOfCurrentMax = byteIndx;
    }
    if( byteFreq[ byteIndx ] < valueOfCurrentMin )  {
      valueOfCurrentMin = byteFreq[ byteIndx ];
      indxOfCurrentMin = byteIndx;
    }
    if( byteFreq[ byteIndx ] == 0L )  zeroFreqCount++;
  }
  *totalCount = total;
  *indxOfMax = indxOfCurrentMax;
  *indxOfMin = indxOfCurrentMin;
  return( zeroFreqCount );
}

void  printFormattedLine( int  indx, long  freq, double  percentageFreq )  {
  printf( "%02x %3d %03o %3s ", indx, indx, indx, humanReadableASCII[ indx ] );
  printf( "%5.1lf%% %ld\n", percentageFreq, freq );
}


int  printByteFrequencies( long  byteFreq[], int  onlyNonZeroFlag )  {
  int  byteIndx;
  int  nonZeroFreqCount = 0;
  long long  byteTotal;
  double  total;
  int  indxOfMax;
  int  indxOfMin;
  int  zeroFreqCount;
  int  dontCalcPercentage;
   
  zeroFreqCount = doByteFreqStats( byteFreq, &byteTotal, &indxOfMax, &indxOfMin );
  /* printf( "Debug: Zero Freq count is %d\n", zeroFreqCount ); */
  /* printf( "Debug: byte Total is %lld\n", byteTotal ); */
  dontCalcPercentage = (( zeroFreqCount == 256 ) || ( byteTotal <= 0LL ));	/* Check all zero array or accumalation error */
  total = ( double ) byteTotal;
  /* printf( "Debug: dontCalcPercentage is %d\n", dontCalcPercentage ); */
  /* printf( "Debug: total is %lg\n", total ); */
/* Only print info for bytes that don't have zero counts if the onlyNonZeroFlag is true */
  for( byteIndx = 0; byteIndx < 256; byteIndx++ )  {
    if( byteFreq[ byteIndx ] != 0L )  {
      printf( "Frequency: " );
      printFormattedLine( byteIndx, byteFreq[ byteIndx ], ( dontCalcPercentage ) ? ( double ) 0.0 : ( double ) byteFreq[ byteIndx ] / ( total * 0.01 ) );
      nonZeroFreqCount += 1;
    }
    else if( ! onlyNonZeroFlag )  {
      printf( "Frequency: " );
      printFormattedLine( byteIndx, byteFreq[ byteIndx ], ( dontCalcPercentage ) ? ( double ) 0.0 : ( double ) byteFreq[ byteIndx ] / ( total * 0.01 ) );
    }
  }
  printf( "Highest:   " );
  printFormattedLine( indxOfMax, byteFreq[ indxOfMax ], ( dontCalcPercentage ) ? ( double ) 0.0 : ( double ) byteFreq[ indxOfMax ] / ( total * 0.01 ) );
  printf( "Lowest:    " );
  printFormattedLine( indxOfMin, byteFreq[ indxOfMin ], ( dontCalcPercentage ) ? ( double ) 0.0 : ( double ) byteFreq[ indxOfMin ] / ( total * 0.01 ) );
  return( nonZeroFreqCount );
}

