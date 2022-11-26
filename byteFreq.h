/*
 * B Y T E F R E Q . H
 *
 * Header for byteFreq.
 *
 * byteFreq.h last edited on Sat Nov 26 22:39:29 2022 
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
 * $Log: byteFreq.h,v $
 * Revision 0.2  2022/11/26 11:43:18  owen
 * Added header info for extra info routines.
 *
 * Revision 0.1  2022/11/20 12:00:21  owen
 * Added cryptogram routine header.
 *
 * Revision 0.0  2022/11/16 12:18:07  owen
 * Initial header info.
 *
 *
 *
 */


void  print_byteFreq_SourceCodeControlIdentifier( void );
void  print_byteFreq_SourceCompileDetails( void );
int  printByteFrequencies( long  byteFreq[], int  onlyNonZeroFlag );
void  printCryptoGramFrequencies( long  byteFreq[] );

