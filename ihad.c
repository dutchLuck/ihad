/*
 * I H A D . C
 *
 * Index Hex Ascii Dump of a (binary) file or stdin.
 *
 * ihad.c last edited on Wed Nov 30 00:15:11 2022 
 *
 * This is not production code! Consider it only slightly tested.
 * Better alternatives are; -
 *
 *  hexdump with Canonical format i.e.  hexdump -C yourFile
 * OR  xxd -g 0 yourFile
 * OR  od -A x -t x1z -v yourFile
 * OR  format-hex on Microsoft Windows in powershell i.e.  format-hex yourFile
 *
 * In addition, PLEASE do not look at the source code as an example
 * of how to code! or how not to code!
 *
 * ihad was written for my own education.
 *
 * This code is released under the MIT license
 *
 * It provides marginally cleaner default output (i.e. just three columns
 * with spaces as column / field delimiters) than the command line
 * utilities listed above.
 */

/*
 * Output a human readable dump of input (bytes) from either stdin or file(s).
 *
 * i.e. read input stream and output lines with the format
 *  index  hex digit 1 .. hex digit N  ascii char 1 .. ascii char N
 *
 * if there is no ascii printable character corresponding to a byte of input
 * then a full stop or a character specified by the -c option is printed.
 *
 * by default space is not considered printable.
 *
 * e.g.
 * $ ./ihad tmp.bin
 * 00000000 54686973206973206120737472696e67 This.is.a.string
 * 00000010 206f6620636861726163746572730a   .of.characters.
 * $
 *
 * Output of the index (first column) may be switched off with the -I switch.
 * Output of the hexadecimal (second column) may be switched off with the -H switch.
 * Output of the ascii (third column) may be switched off with the -A switch.
 *
 * For useage help type the switch -h;
 * e.g.
 *  ./ihad -h
 * 
 */

/*
 * $Log: ihad.c,v $
 * Revision 0.24  2022/11/29 13:15:21  owen
 * Added -L X option to limit dump to a max of X lines.
 *
 * Revision 0.23  2022/11/29 11:32:00  owen
 * Increased information written to file when -o option is used.
 *
 * Revision 0.22  2022/11/28 13:03:09  owen
 * Clarify logic so if unable to determine file size then starting at an offset is ignored.
 *
 * Revision 0.21  2022/11/26 11:31:36  owen
 * Extra debug and ASCII summary information.
 *
 * Revision 0.20  2022/11/23 09:43:01  owen
 * Added -C option to invoke Cryptogram info output mode.
 *
 * Revision 0.19  2022/11/20 11:58:50  owen
 * Added first attempt at cryptogram mode (-v6).
 *
 * Revision 0.18  2022/11/16 12:15:10  owen
 * Added call to external print frequency array.
 *
 * Revision 0.17  2022/11/15 12:15:02  owen
 * Output Summary: info for -v3
 *
 * Revision 0.16  2022/11/10 12:28:49  dutchLuck
 * Fixed -b behaviour when more than 1 file is being dumped.
 *
 * Revision 0.15  2022/10/30 12:08:09  dutchLuck
 * Updated usage to show change to -b option
 *
 * Revision 0.14  2022/10/29 12:12:13  dutchLuck
 * Allowed -b to have negetive values that make the begin point relative to the file end.
 *
 * Revision 0.13  2022/10/28 12:18:01  dutchLuck
 * Limit begin offset to size of files.
 *
 * Revision 0.12  2022/10/28 06:18:40  dutchLuck
 * Fixed -b begin at offset in file if negetive value is specified
 *
 * Revision 0.11  2022/10/23 14:13:51  dutchLuck
 * Added -b X option to begin dumping the file after skipping X bytes.
 *
 * Revision 0.10  2022/10/17 00:18:15  dutchLuck
 * Cleaned up 1 remaining debug statement not prefaced by Debug:
 *
 * Revision 0.9  2022/10/15 06:30:54  dutchLuck
 * Added -S option to specify alternate column separator to space char
 *
 * Revision 0.8  2022/10/12 09:27:42  dutchLuck
 * Added -c option to change character used to indicate non-ASCII bytes
 *
 * Revision 0.7  2022/09/23 14:18:45  owen
 * Made sure multiple input files worked with an output file
 *
 * Revision 0.6  2022/09/23 10:49:36  owen
 * shifted code to make main() easier to understand. Corrected help output.
 *
 * Revision 0.5  2022/09/15 12:35:33  owen
 * Added a -H command line option to disable Hex output column
 *
 * Revision 0.4  2017/11/12 08:32:59  dutchLuck
 * Added the MIT license stipulation into the comments in the top of the file.
 *
 * Revision 0.3  2016/11/26 13:12:37  owen
 * Added -f switch to specify number of spaces between byte hex values.
 *
 * Revision 0.2  2016/11/26 10:44:14  owen
 * Added -s switch to allow spaces as printable chars.
 * changed default format to 3 columns with a single space between columns.
 *
 * Revision 0.1  2016/11/25 02:54:28  owen
 * Added functionality to write output to a file with the -o switch
 *
 * Revision 0.0  2016/11/12 12:05:18  owen
 * Outputs dump to stdout with adjustable width & index & ascii
 * can be suppressed. Doesn't implement all defined switches yet.
 *
 * ihad.c code developed from writeSimpleTemplateCode.c version 0.0.
 *
 */

#include <stdio.h>	/* printf() fopen() perror() fgetc() fclose() fprintf() fseek() */
#include <stdlib.h>	/* atoi() malloc() free() atol() */
#include <unistd.h>	/* getopt() */
#include <string.h>	/* memset() strlen() strdup() */
#include <limits.h>	/* LONG_MIN INT_MIN */
#include <sys/stat.h>	/* fstat() */
#include <ctype.h>	/* isprint() */
#include <libgen.h>	/* basename() */

#include "byteFreq.h"	/* printByteFrequencies() */

#define  SRC_CODE_CNTRL_ID  "$Id: ihad.c,v 0.24 2022/11/29 13:15:21 owen Exp owen $"

#define  BYTE_MASK 0xff
#define  WORD_MASK 0xffff
#define  DEFAULT_WIDTH 16
#define  DEFAULT_DECIMAL_WIDTH 10
#define  MAX_WIDTH 32
#define  HEX_BYTE_FIELD_WIDTH 2		/* Default is 2 which is no spaces */


/* Command line Optional Switches: */
/*  Ascii, beginOffset, charAlternate, Cryptogram, decimal, Debug, fieldSepWidth, */
/*  help, Hex, Index, outFile, columnSeparator, space, verbosity, width */
const char  optionStr[] = "Ab:c:CdDf:hHIL:o:sS:v::w:";

/* Global Flags & Data */
int  A_Flg;			/* Control Ascii column output */
int  bFlg;			/* begin offset bytes from the start of the file */
long  beginOffset;		/* or if < 0 then measure back from end of the file */
char *  bStrng;
int  cFlg;			/* use alternate char instead of full stops for non-ASCII */
char *  cStrng;
char defaultCharStrng[] = ".";
int  C_Flg;			/* Force Cryptogram info output */
int  dFlg;			/* use decimal index and a default hex width of 10 bytes per line */
int  D_Flg;			/* Control Debug info output */
int  fFlg, fieldSeparatorWidth;	/* Control hex field separation width */
char *  fStrng;
int  hFlg;			/* Control help info output */
int  H_Flg;			/* Control Hexadecimal column output */
int  I_Flg;			/* Control Index column output */
int  L_Flg;			/* Limit lines dumped */
long  bytesToDump;		/* number of bytes to dump */
char *  L_Strng;		/* max number of lines to dump */
int  oFlg;			/* Control output file output */
char *  oStrng;		/* output file name */
FILE * ofp;			/* output file pointer */
int  sFlg;			/* Control output of space in ascii column */
char  lowestPrintableAsciiChar;
int  S_Flg;			/* Control Char used between columns */
char *  S_Strng;
char defaultColumnChar[] = " ";
int  vFlg, verbosityLevel;	/* Control verbosity level */
char *  vStrng;
int  wFlg, byteDisplayWidth;	/* Control number of bytes dealt with per line */
char *  wStrng;
char *  exeName;		/* name of this executable */
char *  exePath;		/* path of this executable */

void  setGlobalFlagDefaults( void )  {
/* Preset command line options */
  A_Flg = 0;		/* Default is Ascii column output */
  bFlg = 0;		    /* Default to begin the dump at the beginning of the file */
  beginOffset = 0L;
  bStrng = ( char * ) NULL;
  cFlg = 0;			/* Default to output full-stops */
  cStrng = defaultCharStrng;
  C_Flg = 0;			/* Default to Cryptogram mode off */
  dFlg = 0;			/* Default to output hex index rather than decimal index */
  D_Flg = 0;			/* Default to Debug off */
  fFlg = fieldSeparatorWidth = 0;	/* Default to no spaces between hex bytes */
  fStrng = ( char * ) NULL;
  hFlg = 0;			/* Default to no help text output */
  H_Flg = 0;			/* Default is Hexadecimal column output */
  I_Flg = 0;			/* Default is Index column output */
  L_Flg = 0;			/* Default to unlimited number of dump lines */
  L_Strng = ( char * ) NULL;
  oFlg = 0;			/* Default to output to stdout */
  oStrng = ( char * ) NULL;
  ofp = stdout;		/* Output file pointer defaults to stdout */
  sFlg = 0;			/* Default to replacing space with a '.' in ascii column */
  lowestPrintableAsciiChar = '!';	/* '!' is next to space */
  S_Flg = 0;			/* Default to using space as the column separator */
  S_Strng = defaultColumnChar;
  vFlg = verbosityLevel = 0;	/* Default to no verbose output */
  vStrng = ( char * ) NULL;
  wFlg = 0;			/* Default to DEFAULT_WIDTH bytes per line */
  byteDisplayWidth = DEFAULT_WIDTH;
  wStrng = ( char * ) NULL;
}


void  printOutHelpMessage( char * programName )  {
  printf( "\nUseage:\n" );
  printf( "%s [options] [inputFile1 [inputFile2 [.. inputFileN]]]\n", programName );
  printf( "  where options are '-A -b X -c C -d -D -f X -h -H -I -L X -o outfileName -s -S C -v[X] -w X'; -\n" );
  printf( "   -A .. Ascii output disable\n" );
  printf( "   -b X .. Begin file dumps at an offset (X > 0 offset from start, X < 0 offset from end)\n" );
  printf( "   -c C .. Set char C as non-ASCII indicator & overide -A option\n" );
  printf( "   -C .. Cryptogram mode output enable - i.e. shortcut for -I -H -w32 -v6\n" );
  printf( "   -d .. Decimal index output enable & default to 10 bytes per line\n" );
  printf( "   -D .. Debug output enable\n" );
  printf( "   -f X .. Set hex field separator to X spaces (where 0 < X < 2)\n" );
  printf( "   -h .. Print out this help message and exit\n" );
  printf( "   -H .. Hexadecimal output disable\n" );
  printf( "   -I .. Index output disable\n" );
  printf( "   -L X .. Limit file dumps to a maximum of X lines\n" );
  printf( "   -o outfileName .. Specify an output file instead of sending output to stdout\n" );
  printf( "   -s .. Classify space char as printable in Ascii output\n" );
  printf( "   -S C .. Set char C as column separator\n" );
  printf( "   -v[X] .. Verbose output enable, optionally set level to X (where 0 <= X <= 6)\n" );
  printf( "   -w X .. Set bytes per line to X (where 0 < X <= %d)\n\n", MAX_WIDTH );
  printf( "  where; -\n" );
  printf( "   [inputFile1 [inputFile2 [.. inputFileN]]]  are optional file name(s)\n" );
  printf( "    of file(s) to dump in hex & ascii\n" );
  printf( "   Note that if no input file is specified then input is taken from stdin.\n\n" );
  printf( "Note that if ihad output isn't acceptable you can try; -\nxxd -g 0\nhexdump -C\nod -A x -t x1z -v\n\n" );
}


int  processCommandLineOptions( int  argc, char *  argv[] )  {
  int  result;
  extern char *  optarg;
  extern int  optind, opterr, optopt;

/* Isolate the name of the executable */
  exeName = argv[0];	/* set global variable to default */
  if(( exePath = strdup( argv[0] )) == NULL )
    perror( "?? Unable to create duplicate of path to this executable" );
  else if(( exeName = basename( exePath )) == NULL )  {
    perror( "?? Unable to obtain the name of this executable" );
    exeName = argv[0];	/* set back to the default again */
  }
/* Set all the global flags from command line options */
  opterr = 0;	/* Suppress error messages from getopt() to stderr */
/* Process switch options from the command line */
  while(( result = getopt( argc, argv, optionStr )) != -1 )  {
    switch( result )  {
      case 'A' :  A_Flg = 1; break;
      case 'b' :  bFlg = 1; bStrng = optarg; break;
      case 'c' :  cFlg = 1; cStrng = optarg; break;
      case 'C' :  C_Flg = 1; break;
      case 'd' :  dFlg = 1; break;
      case 'D' :  D_Flg = 1; break;
      case 'f' :  fFlg = 1; fStrng = optarg; break;
      case 'h' :  hFlg = 1; break;
      case 'H' :  H_Flg = 1; break;
      case 'I' :  I_Flg = 1; break;
      case 'L' :  L_Flg = 1; L_Strng = optarg; break;
      case 'o' :  oFlg = 1; oStrng = optarg; break;
      case 's' :  sFlg = 1; lowestPrintableAsciiChar = ' '; break;
      case 'S' :  S_Flg = 1; S_Strng = optarg; break;
      case 'v' :  vFlg = 1; vStrng = optarg; break;
      case 'w' :  wFlg = 1; wStrng = optarg; break;
      default :
        fprintf( stderr, "\n?? command line option '-%c' is unrecognised or incomplete and has been ignored\n", optopt );
        printf( " for help on command line options run '%s -h'\n", exeName );
        break;
    }
  }

/* Postprocess -D (debug) switch option */
  if( D_Flg )  printf( "Debug: Option '-D' is %s (%d)\n", D_Flg ? "True" : "False", D_Flg );
  if( D_Flg )  {
    if( ! vFlg )  {
      vFlg = 1;     /* -v processing still to come will set level to 0 if not specified */
      printf( "Warning: verbose option '-v' set True by -D option\n" );
    }
  }

/* Postprocess -v (verbosityLevel) switch option */
  if( D_Flg )  printf( "Debug: Option '-v' is %s (%d)\n", vFlg ? "True" : "False", vFlg );
  if( vFlg )  {
    if( vStrng == ( char * ) NULL )  {
      if( D_Flg )  printf( "? String for option '-v' is uninitialised, using default value of 0\n" );
      verbosityLevel = 0;
    }
    else  {
      if( D_Flg )  printf( "Debug: String for option '-v' is %s\n", vStrng );
      verbosityLevel = atoi( vStrng );
      if( verbosityLevel > 6 )  verbosityLevel = 6;
      else if( verbosityLevel < 1 )  verbosityLevel = 0; 
    }
    if( D_Flg )  printf( "Debug: verbosity level is %d\n", verbosityLevel );
  }
  else  {
    if( D_Flg )  printf( "Debug: Option '-v' was not found in the command line options\n" );
  }
  
/* Postprocess -f (fieldSeparatorWidth) switch option */
  if( D_Flg )  printf( "Debug: Option '-f' is %s (%d)\n", fFlg ? "True" : "False", fFlg );
  if( fFlg )  {
    if( fStrng == ( char * ) NULL )  {
      printf( "? String for option '-f' is uninitialised, using default value of 1\n" );
      fieldSeparatorWidth = 1;
    }
    else  {
      if( D_Flg )  printf( "Debug: String for option '-f' is %s\n", fStrng );
      fieldSeparatorWidth = atoi( fStrng );
      if( fieldSeparatorWidth > 2 )  fieldSeparatorWidth = 2;
      else if( fieldSeparatorWidth < 0 )  fieldSeparatorWidth = 0; 
    }
    if( D_Flg )  printf( "Debug: field separator width is %d\n", fieldSeparatorWidth );
  }
  else  {
    if( D_Flg )  printf( "Debug: Option '-f' was not found in the command line options\n" );
  }

/* Postprocess -b (begin at offset) switch option */
  if( D_Flg )  printf( "Debug: Option '-b' is %s (%d)\n", bFlg ? "True" : "False", bFlg );
  if( bFlg )  {
    beginOffset = LONG_MIN;     /* set offset to a very unlikely number */
    if( bStrng == ( char * ) NULL )  {
      beginOffset = 0L;
      printf( "? String for option '-b' is uninitialised, using default value of %ld\n", beginOffset );
    }
    else  {
      if( D_Flg )  printf( "Debug: String for option '-b' is %s\n", bStrng );
   /* Convert offset specification to unsigned long */
      beginOffset = atol( bStrng );
      if( D_Flg )  printf( "Debug: The '%s' string for option '-b' was converted to %ld\n", bStrng, beginOffset );
   /* Rough check on atol() output - did it reset beginOffset to zero when option value likely wasn't zero */
      if(( beginOffset == 0L ) && ( *bStrng != '0' ))  {
        fprintf( stderr, "\n?? Unable to convert '%s' into a long integer for option '-b'\n", bStrng );
      }
    }
  }
  else  {
    if( D_Flg )  printf( "Debug: Option '-b' was not found in the command line options\n" );
  }
  if( D_Flg )  printf( "Debug: Begin at an offset in the file of %ld bytes\n", beginOffset );

/* Postprocess -w (width) switch option */
  if( D_Flg )  printf( "Debug: Option '-w' is %s (%d)\n", wFlg ? "True" : "False", wFlg );
  if( wFlg )  {
    byteDisplayWidth = INT_MIN;
    if( wStrng == ( char * ) NULL )  {
      byteDisplayWidth = DEFAULT_WIDTH;
      printf( "? String for option '-w' is uninitialised, using default value of %d\n", byteDisplayWidth );
    }
    else  {
      if( D_Flg )  printf( "Debug: String for option '-w' is %s\n", wStrng );
   /* Convert width specification to integer */
      byteDisplayWidth = atoi( wStrng );
      if( D_Flg )  printf( "Debug: The '%s' string for option '-w' was converted to %d\n", wStrng, byteDisplayWidth );
   /* Rough check on atoi() output - was it a valid conversion */
      if(( byteDisplayWidth == 0 ) && ( *wStrng != '0' ))  {
        fprintf( stderr, "\n?? Unable to convert '%s' to integer for option '-w'\n", wStrng );
      }
      if( byteDisplayWidth > MAX_WIDTH )  byteDisplayWidth = MAX_WIDTH;
    }
  }
  else  {
    if( D_Flg )  printf( "Debug: Option '-w' was not found in the command line options\n" );
    byteDisplayWidth = (( dFlg ) ? DEFAULT_DECIMAL_WIDTH : DEFAULT_WIDTH );
  }
/* Ensure the display width is not zero and not negetive */
  if( byteDisplayWidth < 1 )  {
    byteDisplayWidth = DEFAULT_WIDTH;
    if( wFlg )  fprintf( stderr, "\n?? Reset zero or negetive display width to %d bytes\n", byteDisplayWidth );
  }
  if( D_Flg )  printf( "Debug: byte Display Width is %d\n", byteDisplayWidth );

/* Postprocess -o switch option */
  if( D_Flg )  printf( "Debug: Option '-o' is %s (%d)\n", oFlg ? "True" : "False", oFlg );
  if( oFlg )  {
    if( oStrng == ( char * ) NULL )  {
      printf( "?? File name string for option '-o outfileName' is uninitialised\n" );
      oFlg = 0;
      ofp = stdout;
      fprintf( stderr, "Defaulting to writing output to stdout\n" );
    }
    else  {
      if( D_Flg )  printf( "Debug: File name string part of option '-o' is '%s'\n", oStrng );
    }
  }
  else  {
    if( D_Flg )  printf( "Debug: Option '-o outfileName' was not found in the command line options\n" );
  }

/* Postprocess -c switch option */
  if( D_Flg )  printf( "Debug: Option '-c' is %s (%d)\n", cFlg ? "True" : "False", cFlg );
  if( cFlg )  {
    if( cStrng == ( char * ) NULL )  {
      printf( "?? Character string for option '-c alternateChar' is uninitialised\n" );
      cFlg = 0;
      cStrng  = defaultCharStrng;
      fprintf( stderr, "Defaulting to writing non-ASCII as '.'\n" );
    }
    else  {
      if( D_Flg )  printf( "Debug: Alternate Char string part of option '-c' is '%s'\n", cStrng );
      if( A_Flg )  {
        A_Flg = 0;
        printf( "Warning: -A option reset as a valid -c option overides it\n" );
      }
    }
  }
  else  {
    if( D_Flg )  printf( "Debug: Option '-c alternateChar' was not found in the command line options\n" );
  }
  
/* Postprocess -S switch option */
  if( D_Flg )  printf( "Debug: Option '-S' is %s (%d)\n", S_Flg ? "True" : "False", S_Flg );
  if( S_Flg )  {
    if( S_Strng == ( char * ) NULL )  {
      printf( "?? Character string for option '-S char' is uninitialised\n" );
      S_Flg = 0;
      S_Strng  = defaultColumnChar;
      fprintf( stderr, "Defaulting to space as column separator\n" );
    }
    else  {
      if( D_Flg )  printf( "Debug: Column separator Char string part of option '-S' is '%s'\n", S_Strng );
    }
  }
  else  {
    if( D_Flg )  printf( "Debug: Option '-S char' was not found in the command line options\n" );
  }

/* Postprocess -C (cryptogram mode) switch option */
  if( D_Flg )  printf( "Debug: Option '-C' is %s (%d)\n", C_Flg ? "True" : "False", C_Flg );
  if( C_Flg )  {
    I_Flg = 1;		/* don't print index */
    H_Flg = 1;		/* don't print hex */
    if( ! wFlg )  {	/* if user hasn't set the width then make it the max width */
      wFlg = 1;
      byteDisplayWidth = MAX_WIDTH;
    }
    if( ! vFlg )  {	/* if user hasn't set the verbosity then make it 6 */
      vFlg = 1;
      verbosityLevel = 6;
    }
  }

/* Postprocess -L (dump at most X lines) switch option */
  if( D_Flg )  printf( "Debug: Option '-L' is %s (%d)\n", L_Flg ? "True" : "False", L_Flg );
  if( L_Flg )  {
    bytesToDump = LONG_MAX;     /* set limit to a very large number */
    if( L_Strng == ( char * ) NULL )  {
      printf( "? String for option '-L' is uninitialised, using default value of %ld\n", bytesToDump );
    }
    else  {
      if( D_Flg )  printf( "Debug: String for option '-L' is %s\n", L_Strng );
   /* Convert offset specification to unsigned long */
      bytesToDump = atol( L_Strng );
      if( D_Flg )  printf( "Debug: The '%s' string for option '-L' was converted to %ld\n", L_Strng, bytesToDump );
   /* Rough check on atol() output - did it reset bytesToDump to zero when option value likely wasn't zero */
      if(( bytesToDump == 0L ) && ( *L_Strng != '0' ))  {
        fprintf( stderr, "\n?? Unable to convert '%s' into a long integer for option '-L'\n", L_Strng );
      }
      bytesToDump = byteDisplayWidth * bytesToDump;	/* Convert from max number of lines to max number of bytes */
    }
  }
  else  {
    if( D_Flg )  printf( "Debug: Option '-L' was not found in the command line options\n" );
  }
  if( D_Flg )  printf( "Debug: Limit bytes to Dump from the file to a max of %ld bytes\n", bytesToDump );
 
/* Report on -h switch option */
  if( D_Flg )  printf( "Debug: Option '-h' is %s (%d)\n", hFlg ? "True" : "False", hFlg );
  if( ! hFlg )  {
    if( D_Flg )  printf( "Debug: Option '-h' was not found in the command line options\n" );
  }

/* return the index of the first positional argument (i.e. input file name?) */
  return( optind );
}


long  readByteStreamAndPrintIndexHexAscii( FILE *  fp, FILE *  ofp, long startOffset )  {
  int  byte;
  int  collectSummary;	/* Flag that summary information is required */
  long  byteCnt = 0L;
  long  isasciiCnt = 0L;
  long  isprintCnt = 0L;
  long  iscntrlCnt = 0L;
  long  ispunctCnt = 0L;
  long  isalphaCnt = 0L;
  long  isupperCnt = 0L;
  long  islowerCnt = 0L;
  long  isdigitCnt = 0L;
  long  isspaceCnt = 0L;
  long  freqArray[ 256 ];	/* track frequency of bytes */
  unsigned long  byteAddr = 0L;	/* Index of first byte in each output line */
  char *  outputString;	/* pointer to character string to print to stdout or a file */
  int  outputStringSize;	/* must be big enough for MAX_WIDTH bytes per line */
  char *  bPtr;     /* byte pointer */	/* output line space > 8+2+3*MAX_WIDTH+1+MAX_WIDTH+1 */
  char *  aPtr;     /* ascii pointer */
  int  hexFldWdth;
  char  hexFldFrmt[ 11 ];

/* set or reset flag according to the need to print summary information */
  collectSummary = ( D_Flg || ( verbosityLevel > 1 ) || C_Flg );
  for( byte = 0; byte < 256; byte++ )  freqArray[ byte ] = 0L;
/* Setup the format for printing the hex bytes */
  hexFldWdth = HEX_BYTE_FIELD_WIDTH + fieldSeparatorWidth;
  if( hexFldWdth > ( sizeof( hexFldFrmt ) - 7))  hexFldWdth = ( sizeof( hexFldFrmt ) - 7);
  else if( hexFldWdth < 2 )  hexFldWdth = 2;
  sprintf( hexFldFrmt, "%c02x%c%c%c%c%c", '%', *S_Strng, *S_Strng, *S_Strng, *S_Strng, *S_Strng );	/* Set up a string of the form "%02x  " */
  bPtr = hexFldFrmt + ( 2 + hexFldWdth );	/* Temp use of bPtr to point to end of Hex format string */
  *bPtr = '\0';	/* Terminate Hex format string with the correct number of column separators (Defaults to space) */
/* Get a suitably sized buffer to hold the output string */
  outputStringSize = (( I_Flg ) ? 4 : 14 ) + (( hexFldWdth ) + (( A_Flg ) ? 0 : 1 )) * byteDisplayWidth;
  outputString = malloc( outputStringSize );
  if( outputString == NULL )  {
    fprintf( stderr, "?? malloc( %d ) failed\n", outputStringSize );
    perror( "readByteStreamAndPrintIndexHexAscii()" ); 
  }
  else  {
    if( D_Flg )  {
      fprintf( ofp, "Debug: hexFldWdth is %d and hexFldFrmt is '%s'\n", hexFldWdth, hexFldFrmt ); 
      fprintf( ofp, "Debug: Size of byteAddr (unsigned int) is %lu bytes\n", sizeof( byteAddr ));
      fprintf( ofp, "Debug: Capacity of output string is %d characters\n", outputStringSize );
    }
    *outputString = '\0';		/* in-case the stdin or file has 0 length */
 /* Take any fseek() skip into account if there was one */
    byteAddr = startOffset;
 /* Read bytes from stdin or file until either end of file or bytes to dump reaches zero */
    while((( byte = fgetc( fp )) != EOF ) && (( ! L_Flg ) || ( bytesToDump-- > 0L ))) {
      byte &= BYTE_MASK;
      if( collectSummary )  {
        freqArray[ byte ]++;	/* Accumalate counts of all bytes */
        if( isascii( byte ))  {
          isasciiCnt++;	/* Count ASCII chars as defined by isascii() */
          if( isprint( byte ))  isprintCnt++;	/* Count ASCII printable chars as defined by isprint() i.e. includes space */
          if( isalpha( byte ))  {
            isalphaCnt++;	/* Count ASCII alphabet chars as defined by isalpha() */
            if( isupper( byte ))  isupperCnt++;
            else if( islower( byte ))  islowerCnt++;
          }
          else if( isdigit( byte ))  isdigitCnt++;	/* Count ASCII digit chars as defined by isdigit() */
          else if( ispunct( byte ))  ispunctCnt++;	/* Count ASCII punctuation chars as defined by ispunct() */
          else if( iscntrl( byte ))  iscntrlCnt++;	/* Count ASCII control chars as defined by iscntrl() */
          else if( isspace( byte ))  isspaceCnt++;	/* Count ASCII space chars as defined by isspace() & not control chars */
        }
      }
   /* Initialize new line if required */
      if(( byteCnt++ % byteDisplayWidth ) == 0 )  {
     /* Make the line buffer all column separator characters (defaults to space characters) */
        bPtr = ( char * ) memset( outputString, *S_Strng, outputStringSize );
     /* Put the Index in the line buffer, if not disabled */
        if( ! I_Flg )  {
          sprintf( bPtr, ( dFlg ) ? "%08lu%c" : "%08lx%c", byteAddr, *S_Strng );
	      if( fieldSeparatorWidth > 0 )  sprintf( bPtr + strlen( outputString ), "%s", S_Strng );
          bPtr += strlen( outputString );
        }
        aPtr = bPtr + (( H_Flg ) ? 0 : ( hexFldWdth * byteDisplayWidth + 1 ));
      }
   /* If Hexadecimal output is required put byte into outputString in Hex form */
      if( ! H_Flg )  {
     /* Put byte into outputString in hex form */
        sprintf( bPtr, hexFldFrmt, byte );		/* Print byte as hex number */
        bPtr += hexFldWdth;
      }
   /* If Ascii output is required put byte into outputString in char form */
      if( ! A_Flg )  {
        if(( byte >= lowestPrintableAsciiChar ) && ( byte <= 0x7e ))	/* Test for printable Ascii */
          sprintf( aPtr, "%c", byte );		/* Print byte as ascii character */
        else  *aPtr =  *cStrng;		/* Print a full stop or alternate char instead of non-printable ascii */
        aPtr += 1;
      }
   /* Output current line if required */
      if(( byteCnt % byteDisplayWidth ) == 0 )  {
        if( A_Flg )  sprintf( bPtr, "\n" );	/* terminate line that doesn't have Ascii */
        else  {
          if( ! H_Flg )  *bPtr = *S_Strng;	/* Don't use byte pointer if Hexadecimal is not being output */
          sprintf( aPtr, "\n" );	/* terminate line that does have Ascii */
        }
        if( ! ( I_Flg && H_Flg && A_Flg ))  {	/* Don't print lines of column separators if all 3 columns are off */
          fprintf( ofp, "%s", outputString );	/* print line to output file or stdout */
          if( D_Flg )
            fprintf( ofp, "Debug: Output string is %lu characters\n", strlen( outputString ));
        }
        *outputString = '\0';		/* set string back to zero length */
        byteAddr = startOffset + byteCnt;
      }
    }
 /* Finish off last line if it is a short line */
    if( strlen( outputString ) > 0 )  {
      if( A_Flg )  sprintf( bPtr, "\n" );	/* terminate line that doesn't have Ascii */
      else  {
        if( ! H_Flg )  *bPtr = *S_Strng;	/* Don't use byte pointer if Hexadecimal is not being output */
        sprintf( aPtr, "\n" );	/* terminate line that does have Ascii */
      }
      if( ! ( I_Flg && H_Flg && A_Flg ))  {	/* Don't print last line of column separators if all 3 columns are off */
        fprintf( ofp, "%s", outputString );	/* print line to output file or stdout */
        if( D_Flg )
          fprintf( ofp, "Debug: Output string is %lu characters\n", strlen( outputString ));
      }
    }
    if( outputString != NULL )  {
      free( outputString );
    }
  }
  if( collectSummary )  {
    fprintf( ofp, "Summary: ASCII: %ld chars in total of which %ld are printable (includes spaces)\n", isasciiCnt, isprintCnt );
    fprintf( ofp, "Summary: ASCII: %ld alphabet ( %ld upper case and %ld lower case ) chars\n",
      isalphaCnt, isupperCnt, islowerCnt );
    fprintf( ofp, "Summary: ASCII: %ld digit, %ld punctuation, %ld space and %ld control chars\n",
      isdigitCnt, ispunctCnt, isspaceCnt, iscntrlCnt );
    if(( verbosityLevel > 3 ) || C_Flg )  {	/* Allow user to have both -C and -v4 to get both cryptogram and freq list info */
      if( C_Flg || ( verbosityLevel == 6 ))  printCryptoGramFrequencies( ofp, freqArray );
      if(( verbosityLevel == 4 ) || ( verbosityLevel == 5 ))
        printByteFrequencies( ofp, freqArray, ( verbosityLevel == 4 ));
    }
    else  {
      fprintf( ofp, "Summary: ASCII: %ld space (SP) and %ld horizontal tab (HT) chars\n",
        freqArray[ (int) ' '], freqArray[ (int) '\t'] );
      fprintf( ofp, "Summary: ASCII: %ld carriage return (CR), %ld line feed (LF) and %ld full stop chars\n",
        freqArray[ (int) '\r'], freqArray[ (int) '\n'], freqArray[ (int) '.'] );
    }
  }
  return( byteCnt );
}


int  processA_SingleCommandLineParameter( FILE *  ofp, char *  nameStrng )  {
  int  result;
  long  byteCnt;
  FILE *  fp;
  long  fileSize;
  long  fileOffset;

  if( D_Flg )
    fprintf( ofp, "Debug:  Executing: processA_SingleCommandLineParameter( %s )\n", nameStrng );
/* Keep beginOffset global variable safe from modification in case there are multiple files */
  fileOffset = beginOffset;
/* Open the file for reading (in binary mode) */
  result = (( fp = fopen( nameStrng, "rb" ) ) != NULL );
  if( ! result )  {
    fprintf( stderr, "?? Unable to open a file named '%s' for reading\n", nameStrng );
    perror( "processA_SingleCommandLineParameter()" );
  }
  else  {	/* Begin file opened ok block */
 /* obtain file size for the file that was just opened */
    result = fseek( fp, 0L, SEEK_END );
    if( result != 0 )  {
      fileSize = LONG_MIN;
      fprintf( stderr, "?? Unable to determine file size by seeking to the end of the file named '%s'\n", nameStrng );
      perror( "processA_SingleCommandLineParameter()" );
   /* If there was a problem with the seek make sure it starts at 0 */
      rewind( fp );
      fileOffset = 0L;
    }
    else  {	/* Begin first fseek successful block */
      fileSize = ( long ) ftell( fp );
      /* Ensure file is reset back to starting at 0 */
      rewind( fp );
      if( vFlg )
        fprintf( ofp, "File: '%s' is %ld bytes\n", nameStrng, fileSize );
      if( fileSize == 0L )  {	/* Is there any bytes in the file to process? */
        if( vFlg )
          fprintf( ofp, "? There are no bytes to dump in '%s'\n", nameStrng );
      }
      else if( fileSize < 0L )  {
        fprintf( ofp, "?? There are less than zero (%ld)  bytes to dump in '%s'\n", fileSize, nameStrng );
      }
      else  {	/* Begin file size greater than zero block, so process the file */
        if( D_Flg )
          fprintf( ofp, "Debug: The size of %s is %ld bytes\n", nameStrng, fileSize );
     /* If begin option has been set then seek to the new start */
        if( bFlg )  {
          if( fileOffset > 0L )  {
         /* If fileSize is valid then make sure the seek offset isn't bigger than the file */
            if(( fileSize > 0L ) && ( fileOffset > fileSize ))  fileOffset = fileSize;
         /* Set up the start of the dumping at the begin offset */
            result = fseek( fp, fileOffset, SEEK_SET );
            if( D_Flg )  {
              fprintf( ofp, "Debug: result of fseek() from start of file was %d\n", result );
            }
          }
          else if( fileOffset < 0L )  {
         /* If fileSize is valid then make sure the seek offset isn't bigger than the file */
            if(( fileOffset + fileSize ) < 0L )  fileOffset = -fileSize;
            if( D_Flg )  fprintf( ofp, "Debug: fileOffset for SEEK_END is %ld\n", fileOffset );
         /* Set up to start dumping at the begin offset from the end of the file */
            result = fseek( fp, fileOffset, SEEK_END );
         /* Adjust fileOffset to print correct Index */
            if( result == 0 )  fileOffset += fileSize;
            if( D_Flg )  {
              fprintf( ofp, "Debug: result of fseek() from end of file was %d\n", result );
            }
          }
        }
      }	/* End of file size is greater than 0 block */
    }	/* End first fseek successful block */
 /* Process the file just opened */
    byteCnt = readByteStreamAndPrintIndexHexAscii( fp, ofp, fileOffset );
    result = ( fclose( fp ) == 0 );
    if( ! result )  {
      fprintf( stderr, "?? Unable to close the file named '%s'\n", nameStrng );
      perror( "processA_SingleCommandLineParameter()" );
    }
    if( D_Flg || vFlg )
      fprintf( ofp, "File: '%s' (%ld bytes dumped)\n", nameStrng, byteCnt );
  }	/* End of file opened ok block */
  return( result );
}


int  processNonSwitchCommandLineParameters( int  frstIndx, int  lstIndx, char *  cmdLnStrngs[] )  {
  int  result, indx;
  int  chrCnt;

  result = 0;
  if( D_Flg )  {
    printf( "Debug: Executing: processNonSwitchCommandLineParameters()\n" );
    printf( "Debug: first index is %d and last index is %d\n", frstIndx, lstIndx );
    for( indx = frstIndx; indx <= lstIndx; indx++ )  {
      printf( "Debug: cmdLnStrngs[ %d ] string is '%s'\n", indx, cmdLnStrngs[ indx ] );
    }
  }
  if(( oFlg ) && ( oStrng == ( char * ) NULL ))  {
      fprintf( stderr, "?? -o specified, but no output file name specified - aborting\n" );
  }
  else  {
  /* Attempt to open the output file if required */
    result = ( oFlg ) ? (( ofp = fopen( oStrng, "w" )) != NULL ) : 1;
    if( ! result )  {
      fprintf( stderr, "?? Unable to open a file named '%s' for writing output\n", oStrng );
      perror( "processNonSwitchCommandLineParameters" );
    }
    else  {
      if(( lstIndx + 1 ) == frstIndx )  {
     /* There are no files specified in the command line so process stdin */
        chrCnt = readByteStreamAndPrintIndexHexAscii( stdin, ofp, 0L );
        if( D_Flg || vFlg )  printf( "Processed %d chars from stdin\n", chrCnt );
      }
      else  {
        if( D_Flg )
          printf( "Debug: Largest file size that can be safely handled is %ld bytes\n", LONG_MAX );
     /* Process each file specified in the command line */
        for( indx = frstIndx; indx <= lstIndx; indx++ )  {
          result = processA_SingleCommandLineParameter( ofp, cmdLnStrngs[ indx ] );
        }
      }
   /* Close output file specified in the command line */
      result = ( oFlg ) ? (fclose( ofp ) == 0) : 1;
      if( ! result )  {
        fprintf( stderr, "?? Attempt to close the output file named '%s' failed\n", oStrng );
        perror( "processNonSwitchCommandLineParameters" );
      }
    }
  }
  return( result );
}


int  main( int  argc, char *  argv[] )  {
  int  resultIndex, indx;

/* Preset switch option Flags and Data */
  setGlobalFlagDefaults();

/* Process switch options from the command line */
  resultIndex = processCommandLineOptions( argc, argv );

/* If Debug or verbose options are True then print code details */
  if( D_Flg || (( verbosityLevel > 0 ) && ( verbosityLevel < 3 )))  {
    printf( "Source Code Control Id (RCS) %s\n", SRC_CODE_CNTRL_ID );
    print_byteFreq_SourceCodeControlIdentifier( stdout );
    printf( "Source file %s, compiled on %s at %s\n", __FILE__, __DATE__, __TIME__ );
    print_byteFreq_SourceCompileDetails( stdout );
  }

/* If -h switch option used then print help message and exit */
  if( hFlg )  {
    printOutHelpMessage( exeName );
    return( 1 );
  }

/* If Debug then show rest of non-switch command line options if there are any */
  if( D_Flg )  {
    printf( "Debug: argc is %d and option index is %d\n", argc, resultIndex );
    for( indx = resultIndex; indx < argc; indx++ )  {
      printf( "Debug: argv[ %d ] string is '%s'\n", indx, argv[ indx ] );
    }
  }
  
/* Attempt to dump Index, Hex and Ascii and return 0 if successful */
  return( ! processNonSwitchCommandLineParameters( resultIndex, argc - 1, argv ));
}

