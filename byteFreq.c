/*
 * B Y T E F R E Q . C
 *
 * Support code for Index Hex Ascii Dump (ihad.c)
 *
 * byteFreq.c last edited on Sun Dec 11 23:16:44 2022 
 *
 * byteFreq was written for my own education.
 *
 * This code is released under the MIT license
 *
 */

/*
 * Print routines to list byte frequencies from an array.
 * 
 */

/*
 * $Log: byteFreq.c,v $
 * Revision 0.9  2022/12/11 12:16:56  owen
 * Output an explicit 0x on hex values and a \ on octal values.
 *
 * Revision 0.8  2022/11/29 11:29:02  owen
 * Output to a file, rather than just stdout, if required.
 *
 * Revision 0.7  2022/11/28 13:05:22  owen
 * Switched from array offset use to pointer use.
 *
 * Revision 0.6  2022/11/26 11:41:38  owen
 * Extra information on RCS and compile details.
 *
 * Revision 0.5  2022/11/25 11:44:04  owen
 * Cryptogram freq now sorted with work-around for qsort limitation.
 *
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
 */

#include <stdio.h>	/* printf() fprintf() */
#include <limits.h>	/* LONG_MIN LONG_MAX */
#include <stdlib.h>	/* qsort() */

#define  SRC_CODE_CNTRL_ID  "$Id: byteFreq.c,v 0.9 2022/12/11 12:16:56 owen Exp owen $"

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


void  print_byteFreq_SourceCodeControlIdentifier( FILE *  ofp )  {
  fprintf( ofp, "Source Code Control Id (RCS) %s\n", SRC_CODE_CNTRL_ID );
}


void  print_byteFreq_SourceCompileDetails( FILE *  ofp )  {
  fprintf( ofp, "Source file %s, compiled on %s at %s\n", __FILE__, __DATE__, __TIME__ );
}


int  doByteFreqStats( long *  byteFreq, long long *  totalCount, int *  indxOfMax, int *  indxOfMin )  {
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
    total += (long long) *byteFreq;
    if( *byteFreq > valueOfCurrentMax )  {
      valueOfCurrentMax = *byteFreq;
      indxOfCurrentMax = byteIndx;
    }
    if( *byteFreq < valueOfCurrentMin )  {
      valueOfCurrentMin = *byteFreq;
      indxOfCurrentMin = byteIndx;
    }
    if( *byteFreq++ == 0L )  zeroFreqCount++;
  }
  *totalCount = total;
  *indxOfMax = indxOfCurrentMax;
  *indxOfMin = indxOfCurrentMin;
  return( zeroFreqCount );
}


/*
 * Print Hex, Dec, Octal & ASCII representation of a byte
 * then the frequency as a percentage and then as a count.
 */
 
void  printFormattedLine( FILE *  ofp, int  indx, long  freq, double  percentageFreq )  {
  fprintf( ofp, "0x%02x %3d \\%03o %3s ", indx, indx, indx, humanReadableASCII[ indx ] );
  fprintf( ofp, "%5.1lf%% %ld\n", percentageFreq, freq );
}


int  printByteFrequencies( FILE *  ofp, long  byteFreq[], int  onlyNonZeroFlag )  {
  int  byteIndx;
  int  nonZeroFreqCount = 0;
  long long  byteTotal;
  double  total, percentMult;
  int  indxOfMax;
  int  indxOfMin;
  int  zeroFreqCount;
  int  dontCalcPercentage;
   
  zeroFreqCount = doByteFreqStats( byteFreq, &byteTotal, &indxOfMax, &indxOfMin );
  /* fprintf( ofp, "Debug: Zero Freq count is %d\n", zeroFreqCount ); */
  /* fprintf( ofp, "Debug: byte Total is %lld\n", byteTotal ); */
  dontCalcPercentage = (( zeroFreqCount == 256 ) || ( byteTotal <= 0LL ));	/* Check all zero array or accumulation error */
  total = ( double ) byteTotal;
  percentMult = ( double ) 100.0 / total;	/* repeated multiply in loop is cheaper than divide */
  /* fprintf( ofp, "Debug: dontCalcPercentage is %d\n", dontCalcPercentage ); */
  /* fprintf( ofp, "Debug: total is %lg\n", total ); */
/* Only print info for bytes that don't have zero counts if the onlyNonZeroFlag is true */
  for( byteIndx = 0; byteIndx < 256; byteIndx++ )  {
    if( byteFreq[ byteIndx ] != 0L )  {
      fprintf( ofp, "Frequency: " );
      printFormattedLine( ofp, byteIndx, byteFreq[ byteIndx ], ( dontCalcPercentage ) ? ( double ) 0.0 : ( double ) byteFreq[ byteIndx ] * percentMult );
      nonZeroFreqCount += 1;
    }
    else if( ! onlyNonZeroFlag )  {
      fprintf( ofp, "Frequency: " );
      printFormattedLine( ofp, byteIndx, byteFreq[ byteIndx ], ( dontCalcPercentage ) ? ( double ) 0.0 : ( double ) byteFreq[ byteIndx ] * percentMult );
    }
  }
  fprintf( ofp, "Highest:   " );
  printFormattedLine( ofp, indxOfMax, byteFreq[ indxOfMax ], ( dontCalcPercentage ) ? ( double ) 0.0 : ( double ) byteFreq[ indxOfMax ] * percentMult );
  fprintf( ofp, "Lowest:    " );
  printFormattedLine( ofp, indxOfMin, byteFreq[ indxOfMin ], ( dontCalcPercentage ) ? ( double ) 0.0 : ( double ) byteFreq[ indxOfMin ] * percentMult );
  return( nonZeroFreqCount );
}


/*
 * Compare routine that is used by qsort() routine
 */
 
int  longCompare( const void *  p1, const void *  p2 )  {
  long *  l1;
  long *  l2;
  
  l1 = ( long * ) p1;
  l2 = ( long * ) p2;
  if( *l1 > *l2 )  return -1;
  else if( *l1 < *l2 )  return 1;
  return 0;
}


void  printCryptoGramFrequencies( FILE * ofp, long  byteFreq[] )  {
  long  cryptoFreq[ 256 ];
  long  largestFreq;
  long long  charTotal;
  long *  lPtr;
  double  total, percentMult;
  int  indx;
  long  sortFreqAscii[ 26 ];	/* Combine Freq with associated Char to sort */

/* Cryptograms typically are all upper case and spaces, but may or may not have punctuation */  
  for( indx = 0; indx < 256; indx++ )  cryptoFreq[ indx ] = 0L;	/* Zero the array */
  largestFreq = byteFreq[ (int) 'A' ];
  charTotal = 0LL;
  /* Only really interested in A to Z + a to z so don't bother with the rest of the byteFreq array */
  for( indx = (int) 'A', lPtr = cryptoFreq + indx; indx <= (int) 'Z'; indx++ )  {
    *lPtr = byteFreq[ indx ] + byteFreq[ indx + 32 ];	/* Put both upper and lower case into uppercase freq */
    if( *lPtr > largestFreq )  largestFreq = *lPtr;
    charTotal += (long long) *lPtr++;
  }
  if( charTotal == 0LL )
    fprintf( ofp, "? There are no characters in the range of A to Z or a to z to print\n" );
  else if( charTotal < 0LL )
    fprintf( ofp, "?? %lld characters in the range of A to Z or a to z, which shouldn't occur\n",  charTotal );
  else  {
    total = ( double ) charTotal;
    percentMult = ( double ) 100.0 / total;
    /* Print 2 line horizontal list of non-zero A to Z frequencies */
    fprintf( ofp, "Char: " );
    /* Print a line of A to Z Characters */
    for( indx = (int) 'A'; indx <= (int) 'Z'; indx++ )
      fprintf( ofp, " %c ", ( char ) indx );	/* print A to Z chars */
    fprintf( ofp, "\n%%Frq: " );
    /* Print a line of % frequencies associated with the Character on the line above it */
    for( indx = (int) 'A'; indx <= (int) 'Z'; indx++ )
      fprintf( ofp, "%2.0lf ", ( double ) cryptoFreq[ indx ] * percentMult );
    fprintf( ofp, "\n" );
    /* Print 2 line horizontal list of frequencies sorted highest to lowest  */
    /* Where two or more frequencies are the same ensure associated chars are in alphabetic order  */
    if( largestFreq > ( LONG_MAX >> 5 ))  {
      fprintf( ofp, "? Unable to sort frequency values due to size of largest frequency (%ld)\n", largestFreq );
    }
    else  {
      fprintf( ofp, "\n%%Frq: " );
      /* Cryptograms are generally short sayings, so can have multiple freq values the same */
      /* and it probably isn't going to matter that we don't have the full long capacity any more */
      for( indx = 0, lPtr = cryptoFreq + ( long ) 'A'; indx < 26; indx++ )
        sortFreqAscii[ indx ] = ( *lPtr++ << 5 ) + ( 0x1f & ~(indx + (int) 'A'));	/* combine freq & char so that both are sorted */
      qsort( sortFreqAscii, 26, sizeof( long ), &longCompare );
      /* Print a line of % frequencies */
      for( indx = 0; indx < 26; indx++ )
        fprintf( ofp, "%2.0lf ", ( double ) ( sortFreqAscii[ indx ] >> 5 ) * percentMult );	/* recover just freq part and print % freq */
      fprintf( ofp, "\nChar: " );
      /* Print a line of Characters associated with the frequency on the line above it */
      for( indx = 0; indx < 26; indx++ )  
        fprintf( ofp, " %c ", ( char ) (( 0x1f & (~ sortFreqAscii[ indx ])) | 0x40));	/* recover char part associated with freq and print it */
      fprintf( ofp, "\n" );
    }
  }
}

