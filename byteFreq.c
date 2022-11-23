/*
 * B Y T E F R E Q . C
 *
 * Index Hex Ascii Dump of a (binary) file or stdin.
 *
 * byteFreq.c last edited on Wed Nov 23 20:42:53 2022 
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
 * Revision 0.4  2022/11/23 09:44:13  owen
 * Changed cryptogram freq sort to use A to Z as an index and sort it.
 *
 * Revision 0.3  2022/11/22 11:58:21  owen
 * Added sorted frequency list to cryptogram routine.
 *
 * Revision 0.2  2022/11/20 11:59:58  owen
 * Added first attempt at cryptogram routine.
 *
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
#include <stdlib.h> /* qsort() */

#define  SRC_CODE_CNTRL_ID  "$Id: byteFreq.c,v 0.4 2022/11/23 09:44:13 owen Exp owen $"

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

/* define storage that allows index to this array to be sorted */
int  globalFreq[ 26 ];


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
  dontCalcPercentage = (( zeroFreqCount == 256 ) || ( byteTotal <= 0LL ));	/* Check all zero array or accumulation error */
  total = ( double ) byteTotal;
  /* printf( "Debug: dontCalcPercentage is %d\n", dontCalcPercentage ); */
  /* printf( "Debug: total is %lg\n", total ); */
/* Only print info for bytes that don't have zero counts if the onlyNonZeroFlag is true */
  for( byteIndx = 0; byteIndx < 256; byteIndx++ )  {
    if( byteFreq[ byteIndx ] != 0L )  {
      printf( "Frequency: " );
      printFormattedLine( byteIndx, byteFreq[ byteIndx ], ( dontCalcPercentage ) ? ( double ) 0.0 : ( double ) byteFreq[ byteIndx ] / ( total * 0.01 ));
      nonZeroFreqCount += 1;
    }
    else if( ! onlyNonZeroFlag )  {
      printf( "Frequency: " );
      printFormattedLine( byteIndx, byteFreq[ byteIndx ], ( dontCalcPercentage ) ? ( double ) 0.0 : ( double ) byteFreq[ byteIndx ] / ( total * 0.01 ));
    }
  }
  printf( "Highest:   " );
  printFormattedLine( indxOfMax, byteFreq[ indxOfMax ], ( dontCalcPercentage ) ? ( double ) 0.0 : ( double ) byteFreq[ indxOfMax ] / ( total * 0.01 ));
  printf( "Lowest:    " );
  printFormattedLine( indxOfMin, byteFreq[ indxOfMin ], ( dontCalcPercentage ) ? ( double ) 0.0 : ( double ) byteFreq[ indxOfMin ] / ( total * 0.01 ));
  return( nonZeroFreqCount );
}


int  freqCompare( const void *  p1, const void *  p2 )  {
  int *  intPtr1;
  int *  intPtr2;
  long  freq1;
  long  freq2;
  
  intPtr1 = ( int * ) p1;
  intPtr2 = ( int * ) p2;
  freq1 = globalFreq[ *intPtr1 - (int) 'A' ];
  freq2 = globalFreq[ *intPtr2 - (int) 'A' ];
  if( freq1 > freq2 )  return -1;
  else if( freq1 < freq2 )  return 1;
  return 0;
}


int  longCompare( const void *  p1, const void *  p2 )  {
  long *  l1;
  long *  l2;
  
  l1 = ( long * ) p1;
  l2 = ( long * ) p2;
  if( *l1 > *l2 )  return -1;
  else if( *l1 < *l2 )  return 1;
  return 0;
}


void  printCryptoGramFrequencies( long  byteFreq[] )  {
  long  cryptoFreq[ 256 ];
  long long  charTotal;
  double  total;
  int  indx;
  int  sortAscii[ 26 ];	/* A .. Z index to sort */
  
  for( indx = 0; indx < 256; indx++ )  cryptoFreq[ indx ] = 0L;	/* Zero the array */
  charTotal = 0LL;
  for( indx = (int) 'A'; indx <= (int) 'Z'; indx++ )  {
    cryptoFreq[ indx ] = byteFreq[ indx ] + byteFreq[ indx + 32 ];	/* Put both upper and lower case into uppercase freq */
    charTotal += (long long) cryptoFreq[ indx ];
  }
  if( charTotal > 0LL )  {
    total = ( double ) charTotal;
    /* Print 2 line horizontal list of non-zero A to Z frequencies */
    printf( "Char: " );
    /* Print a line of A to Z Characters */
    for( indx = (int) 'A'; indx <= (int) 'Z'; indx++ )
      if( cryptoFreq[ indx ] > 0L )  printf( " %c ", ( char ) indx );	/* print chars with non-zero freqs */
    printf( "\n%%Frq: " );
    /* Print a line of % frequencies associated with the Character on the line above it */
    for( indx = (int) 'A'; indx <= (int) 'Z'; indx++ )
      if( cryptoFreq[ indx ] > 0L )
        printf( "%2.0lf ", ( double ) cryptoFreq[ indx ] / ( total * 0.01 ));
    /* Print 2 line horizontal list of frequencies sorted highest to lowest  */
    printf( "\n\n%%Frq: " );
    for( indx = 0; indx < 26; indx++ )  {
      sortAscii[ indx ] = indx + (int) 'A';	/* set up index values that can be sorted */
      globalFreq[ indx ] = cryptoFreq[ indx + (int) 'A' ];	/* set up freq data that can be seen by freqCompare() */
    }
    qsort( sortAscii, 26, sizeof( int ), &freqCompare );
    /* Print a line of % frequencies */
    for( indx = 0; indx < 26; indx++ )
      printf( "%2.0lf ", ( double ) cryptoFreq[ sortAscii[ indx ]] / ( total * 0.01 ));
    printf( "\nChar: " );
    /* Print a line of Characters associated with the frequency on the line above it */
    for( indx = 0; indx < 26; indx++ )  
      printf( " %c ", ( char ) sortAscii[ indx ]);
    printf( "\n" );
  }
}

