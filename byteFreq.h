/*
 * B Y T E F R E Q . H
 *
 * Header for byteFreq.
 *
 * byteFreq.h last edited on Tue Nov 29 22:28:48 2022 
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
 * Revision 0.3  2022/11/29 11:31:37  owen
 * Changed routine prototypes to include file variable.
 *
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


void  print_byteFreq_SourceCodeControlIdentifier( FILE * ofp  );
void  print_byteFreq_SourceCompileDetails( FILE * ofp  );
int  printByteFrequencies( FILE * ofp, long  byteFreq[], int  onlyNonZeroFlag );
void  printCryptoGramFrequencies( FILE * ofp, long  byteFreq[] );

