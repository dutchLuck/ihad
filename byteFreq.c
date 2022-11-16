/*
 * B Y T E F R E Q . C
 *
 * Index Hex Ascii Dump of a (binary) file or stdin.
 *
 * byteFreq.c last edited on Wed Nov 16 23:14:50 2022 
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
 * Revision 0.0  2022/11/16 12:24:45  owen
 * Print each byte frequency on a separate line.
 *
 *
 */

#include <stdio.h>	/* printf() fprintf() */

#define  SRC_CODE_CNTRL_ID  "$Id: byteFreq.c,v 0.0 2022/11/16 12:24:45 owen Exp owen $"

#define  BYTE_MASK 0xff

int  printByteFrequencies( long  byteFreq[], int  onlyNonZeroFlag )  {
  int  byteIndx;
  int  nonZeroFreqCount = 0;

  for( byteIndx = 0; byteIndx < 256; byteIndx++ )  {
    if( byteFreq[ byteIndx ] != 0L )  {
        printf( "Frequency: %02x %03d %03o -> %ld\n", byteIndx, byteIndx, byteIndx, byteFreq[ byteIndx ] );
        nonZeroFreqCount += 1;
    }
    else if( ! onlyNonZeroFlag )  {
      printf( "Frequency: %02x %03d %03o -> %ld\n", byteIndx, byteIndx, byteIndx, byteFreq[ byteIndx ] );
    }
  }
  return( nonZeroFreqCount );
}

