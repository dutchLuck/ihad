/*
 * B Y T E F R E Q . H
 *
 * Header for byteFreq.
 *
 * byteFreq.h last edited on Sun Nov 20 22:58:41 2022 
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
 * Revision 0.1  2022/11/20 12:00:21  owen
 * Added cryptogram routine header.
 *
 * Revision 0.0  2022/11/16 12:18:07  owen
 * Initial header info.
 *
 *
 *
 */


int  printByteFrequencies( long  byteFreq[], int  onlyNonZeroFlag );
void  printCryptoGramFrequencies( long  byteFreq[] );

