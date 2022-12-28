/*
 * I H A D . C
 *
 * Index Hex Ascii Dump of a (binary) file or stdin.
 *
 * ihad.c last edited on Wed Dec 28 23:19:30 2022 
 *
 * Industry standard dump alternatives to ihad are; -
 *  hexdump with Canonical format i.e.  hexdump -C yourFile
 * OR  xxd -g 0 yourFile
 * OR  od -A x -t x1z -v yourFile
 * OR  format-hex on Microsoft Windows in powershell i.e.  format-hex yourFile
 *
 * ihad provides marginally cleaner default output (i.e. just three columns
 * with spaces as column / field delimiters) than the command line
 * utilities listed above.
 *
 * This code is released under the MIT license
 *
 * ihad was written for my own education, but please do not look at the source code
 * as an example of how to code! or how not to code!
 *
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
 * Compile limited DEBUG version with; -
 *  cc -Wall -c -UDEBUG byteFreq.c
 *  cc -Wall -c -UDEBUG ihad.c
 *  cc -o ihad ihad.o byteFreq.o
 *
 * Compile full DEBUG version with; -
 *  cc -Wall -c -DDEBUG byteFreq.c
 *  cc -Wall -c -DDEBUG ihad.c
 *  cc -o ihad ihad.o byteFreq.o
 *
 */

/*
 * $Log: ihad.c,v $
 * Revision 0.41  2022/12/28 12:19:38  owen
 * Updated Useage help message.
 *
 * Revision 0.40  2022/12/28 09:11:28  owen
 * Ensure '-' as the file name is taken as accept input from stdin.
 *
 * Revision 0.39  2022/12/27 12:08:09  owen
 * Fixed -L and -B options to be per input file, if there a multiple input files.
 *
 * Revision 0.38  2022/12/23 12:17:58  owen
 * Ensured all string quotes are double quote delimited.
 *
 * Revision 0.37  2022/12/21 11:57:00  owen
 * Improved return code handling.
 *
 * Release Version 0.1.0-beta
 *
 * Revision 0.36  2022/12/20 12:21:56  owen
 * Removed unused code, ensured double quotes on strings, ensured Warning: replaced ??
 *
 * Revision 0.35  2022/12/19 12:20:31  owen
 * Limited -D debug print statements unless compiled with -DDEBUG.
 *
 * Revision 0.34  2022/12/18 11:01:19  owen
 * Ensure -c and -S option char values are limited to a suitable range.
 *
 * Revision 0.33  2022/12/17 10:33:00  owen
 * Changed invalid number input message from stderr to stdout.
 *
 * Revision 0.32  2022/12/13 12:32:26  owen
 * Removed redundent initialization of global variables.
 *
 * Revision 0.31  2022/12/11 12:18:54  owen
 * Options -c and -S have more ways to specify the character to use.
 *
 * Revision 0.30  2022/12/09 13:26:27  owen
 * Reworked -c C and -S C option processing.
 *
 * Revision 0.29  2022/12/07 13:01:10  owen
 * Replaced verbose message about bytes dumped to bytes processed.
 *
 * Revision 0.28  2022/12/06 12:37:49  owen
 * Reworked option -v, -f and -w processing to use subroutines.
 *
 * Revision 0.27  2022/12/02 12:57:35  owen
 * Added limits to -L and -B options to prevent negetive values being accepted.
 *
 * Revision 0.26  2022/12/01 13:33:22  owen
 * Use common routine for -b -B and -L value acquisition.
 *
 * Revision 0.25  2022/11/30 11:17:15  owen
 * Added "-B X" option to limit bytes dumped to a maximum of X bytes.
 *
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
#include <string.h>	/* memset() strlen() strdup() strncmp() */
#include <limits.h>	/* LONG_MIN LONG_MAX INT_MIN */
#include <sys/stat.h>	/* fstat() */
#include <ctype.h>	/* isascii(), isprint(), isalpha(), isdigit() */
#include <libgen.h>	/* basename() */

#include "byteFreq.h"	/* printByteFrequencies() print_byteFreq_SourceCodeControlIdentifier() */

#define  SRC_CODE_CNTRL_ID  "$Id: ihad.c,v 0.41 2022/12/28 12:19:38 owen Exp owen $"

#ifndef FALSE
#define  FALSE 0
#endif
#ifndef TRUE
#define  TRUE (! FALSE)
#endif
#define  BYTE_MASK 0xff
#define  MIN_VERBOSITY_LEVEL 0
#define  MAX_VERBOSITY_LEVEL 6
#define  MIN_HEX_FIELD_SEPARATOR_WIDTH 0
#define  MAX_HEX_FIELD_SEPARATOR_WIDTH 2
#define  DEFAULT_BYTES_PER_LINE 16
#define  DEFAULT_DECIMAL_BYTES_PER_LINE 10
#define  MAX_BYTES_PER_LINE 32
#define  MIN_BYTES_PER_LINE 1
#define  HEX_BYTE_FIELD_WIDTH 2		/* Default is 2 which is no spaces */
#define  DEFAULT_COLUMN_SEPARATOR_CHAR ' '	/* Default to space (SP) char */
#define  DEFAULT_COLUMN_SEPARATOR_STRING " "	/* Default to space (SP) char */
#define  DEFAULT_NON_PRINTABLE_ASCII_INDICATOR_CHAR '.'	/* Default to full stop (i.e. period) char */
#define  DEFAULT_NON_PRINTABLE_ASCII_INDICATOR_STRING "."	/* Default to full stop (i.e. period) char */


/* Command line Optional Switches: */
/*  Ascii, beginOffset, ByteCount, charAlternate, Cryptogram, decimal, Debug, */
/*  fieldSepWidth, help, Hex, Index, outFile, columnSeparator, space, verbosity, width */
const char  optionStr[] = "Ab:B:c:CdDf:hHIL:o:sS:v::w:";

/* Global Flags & default data */
int  A_Flg = FALSE;		/* ASCII option: Control Ascii column output */
int  bFlg = FALSE;		/* begin option: begin offset bytes from the start of the file */
long  beginOffset = 0L;	/*  or if < 0 then measure back from end of the file */
char *  bStrng = ( char * ) NULL;
int  B_Flg = FALSE;		/* Bytes option: Limit bytes dumped */
long  bytesToDump = LONG_MAX;	/*  number of bytes to dump */
char *  B_Strng = ( char * ) NULL;	/*  pointer to -B value (max number of bytes to dump) */
int  cFlg = FALSE;		/* char option: use alternate char instead of full stops for non-ASCII or non-printable ASCII */
char *  cStrng = ( char * ) NULL;	/*  pointer to -c value if there is a value */
char cCharStrng[] = DEFAULT_NON_PRINTABLE_ASCII_INDICATOR_STRING;	/*  use full stop (i.e. period) if no alternate is specified in the options */
int  C_Flg = FALSE;		/* Cryptogram option: force Cryptogram info output */
int  dFlg = FALSE;		/* decimal option: use decimal index and a default hex width of 10 bytes per line */
int  D_Flg = FALSE;		/* Debug option: Control Debug info output */
int  fFlg = FALSE;		/* field option: Control hex field separation width */
int  fieldSeparatorWidth= MIN_HEX_FIELD_SEPARATOR_WIDTH;	/* Default to no spaces between hex bytes */
char *  fStrng = ( char * ) NULL;
int  hFlg = FALSE;		/* help option: Control help info output */
int  H_Flg = FALSE;		/* Hex option: Control Hexadecimal column output */
int  I_Flg = FALSE;		/* Index option: Control Index column output */
int  L_Flg = FALSE;		/* Limit option: Limit lines dumped */
long  linesToDump = LONG_MAX;	/*  number of lines to dump */
char *  L_Strng = ( char * ) NULL;	/*  pointer to -L value (max number of lines to dump) */
int  oFlg = FALSE;		/* output option: Control output file output */
char *  oStrng = ( char * ) NULL;	/*  pointer to -o string (output file name) */
FILE * ofp;			/*  output file pointer */
int  sFlg = FALSE;		/* space option: Control whether space is considered printable in ascii column */
char  lowestPrintableAsciiChar = '!';	/* '!' is next to space */
int  S_Flg = FALSE;		/* Separator option: Allow alternate Char use between columns */
char *  S_Strng = ( char * ) NULL;	/*  pointer to -S (Separator) value if there is a value */
char S_ColumnChar[] = DEFAULT_COLUMN_SEPARATOR_STRING;	/*  use space char if no alternate is specified in the options */
int  vFlg = FALSE;		/* verbosity option: Control verbosity level */
int  verbosityLevel = 0;	/*  Control verbosity level */
char *  vStrng = ( char * ) NULL;	/*  pointer to -v value string, if there is one */
int  wFlg = FALSE;		/* width option:  Control number of bytes dealt with per line */
int  byteDisplayWidth = DEFAULT_BYTES_PER_LINE;	/*  Control number of bytes dealt with per line */
char *  wStrng = ( char * ) NULL;	/*  pointer to -w value string */
char *  exeName;		/* name of this executable */
char *  exePath = ( char * ) NULL;	/* path of this executable */


void  setGlobalFlagDefaults( char *  argv[] )  {
/* Preset command line options */
  ofp = stdout;		/* Output file pointer defaults to stdout */
  exeName = argv[ 0 ];		/* Point to full path to exe as a default */
}


/*
 * long  limitLongValueToEqualOrMoreNegettiveThan( long  value, long  boundary )  {
 *  return(( value > boundary ) ? boundary : value );
 * }
 */


long  limitLongValueToEqualOrMorePositiveThan( long  value, long  boundary )  {
  return(( value < boundary ) ? boundary : value );
}


long  limitLongValueToEqualOrWithinRange( long  value, long  loBoundary, long  hiBoundary )  {
  return(( value < loBoundary ) ? loBoundary : (( value > hiBoundary ) ? hiBoundary : value ));
}


long  convertOptionStringToLong( long  defltValue, char *  strng, char *  flgName, int *  flgActive )  {
  long  result;

  result = defltValue;
  if( strng == ( char * ) NULL )  {
    *flgActive = FALSE;
    printf( "Warning: Parameter value for option %s is uninitialised, ignoring option %s\n", flgName, flgName );
  }
  else if( *strng == '\0' )  {	/* Assume "" used as option value so return default */
    *flgActive = FALSE;
    printf( "Warning: Parameter value for option %s contains no information, ignoring option %s\n", flgName, flgName );
  }
  else  {
#ifdef DEBUG  
    if( D_Flg )  printf( "Debug: String for option %s is \"%s\"\n", flgName, strng );
#endif
 /* Convert option string specified to signed long, if possible */
    result = atol( strng );
 /* Rough check on atol() output - is result zero when option string likely wasn't zero */
    if(( result == 0L ) && ( *strng != '0' ))  {
      *flgActive = FALSE;
      result = defltValue;
      printf( "Warning: Unable to convert \"%s\" into an integer for option %s, ignoring option %s\n",
        strng, flgName, flgName );
    }
#ifdef DEBUG  
    if( D_Flg )  printf( "Debug: The conversion of string \"%s\" for option %s resulted in %ld\n", strng, flgName, result );
#endif
  }
  return( result );
}


int  limitIntegerValueToEqualOrWithinRange( int  value, int  loBoundary, int  hiBoundary )  {
  return(( value < loBoundary ) ? loBoundary : (( value > hiBoundary ) ? hiBoundary : value ));
}


int  convertOptionStringToInteger( int  defltValue, char *  strng, char *  flgName, int *  flgActive )  {
  long  tempResult;
  int  result;

  tempResult = convertOptionStringToLong(( long ) defltValue, strng, flgName, flgActive );	/* use long int code */
  result = ( int ) limitLongValueToEqualOrWithinRange( tempResult, ( long ) INT_MIN, ( long ) INT_MAX );	/* ensure result fits in an integer */
  return( result );
}


unsigned char  limitUnsignedCharValueToEqualOrWithinRange( unsigned char  value, unsigned char  loBoundary, unsigned char  hiBoundary )  {
  return(( value < loBoundary ) ? loBoundary : (( value > hiBoundary ) ? hiBoundary : value ));
}


unsigned char  convertOptionStringToByteChar( int *  flgPtr, char *  defltValue, char *  strng, char *  flgName )  {
  unsigned char  result;
  int  strngLength;

  result = ( unsigned char ) *defltValue;
  strngLength = strlen( strng );
  if( strng == ( char * ) NULL )  {
    printf( "Warning: Parameter value for option %s is uninitialised, using default value of '%c'\n", flgName, result );
    *flgPtr = FALSE;
  }
  else  {
#ifdef DEBUG  
    if( D_Flg )  printf( "Debug: String for option %s is \"%s\"\n", flgName, strng );
#endif
    if( strngLength == 0 )  {	/* Assume "" used so return default */
      printf( "Warning: Parameter value for option %s contains no information, using default value of '%c'\n", flgName, ( char ) result );
      *flgPtr = FALSE;
    }
    else if( strngLength == 1 )  {	/* Assume this single byte is the character to use */
      result = ( unsigned char ) *strng;
    }
    else if( *strng == '\\' )  {	/* Look for a single backslash char as indicator of tr utility style input */
      if( strngLength == 2 )  {	/* Assume \t style char specifier like the translate utility */
        switch( strng[ 1 ] )  {
          case '\\' : result = ( unsigned char ) 0x5c; break;	/* backslash - used as the escape indicator */
          case '0'  : result = ( unsigned char ) 0x00; break;	/* Single digit representation of NUL */
          case '1'  : result = ( unsigned char ) 0x01; break;	/* Single digit representation of SOH */
          case '2'  : result = ( unsigned char ) 0x02; break;	/* Single digit representation of STX */
          case '3'  : result = ( unsigned char ) 0x03; break;	/* Single digit representation of ETX */
          case '4'  : result = ( unsigned char ) 0x04; break;	/* Single digit representation of EOT */
          case '5'  : result = ( unsigned char ) 0x05; break;	/* Single digit representation of ENQ */
          case '6'  : result = ( unsigned char ) 0x06; break;	/* Single digit representation of ACK */
          case '7'  : result = ( unsigned char ) 0x07; break;	/* Single digit representation of BEL */
          case 'a'  : result = ( unsigned char ) 0x07; break;	/* BEL */
          case 'b'  : result = ( unsigned char ) 0x08; break;	/* BS */
          case 'f'  : result = ( unsigned char ) 0x0c; break;	/* FF */
          case 'n'  : result = ( unsigned char ) 0x0a; break;	/* LF */
          case 'r'  : result = ( unsigned char ) 0x0d; break;	/* CR */
          case 't'  : result = ( unsigned char ) 0x09; break;	/* HT */
          case 'v'  : result = ( unsigned char ) 0x0b; break;	/* VT */
        }
      }
      else if( strngLength > 2 )  {	/* Assume \NNN octal style char specifier like the translate utility */
        result = ( unsigned char ) 0;
        if(( strng[ 1 ] >= '0' ) && ( strng[ 1 ] <= '7' ))  {	/* Accept any octal digit */
          result = ( strng[ 1 ] - '0' );
        }
        if(( strng[ 2 ] >= '0' ) && ( strng[ 2 ] <= '7' ))  {	/* Accept any octal digit */
          result = ( result << 3 ) + ( strng[ 2 ] - '0' );
        }
        if(( strngLength > 3 ) && (( strng[ 3 ] >= '0' ) && ( strng[ 3 ] <= '7' )))  {	/* Accept any octal digit */
          result = ( result << 3 ) + ( strng[ 3 ] - '0' );
        }
      }
    }
    else  {
   /* Convert option string specified to (unsigned) integer, if possible */
      result = ( unsigned char ) ( atoi( strng ) & BYTE_MASK );
   /* Rough check on atoi() output - is result zero when option string likely wasn't zero */
      if(( result == 0 ) && ( *strng != '0' ))  {
        printf( "Warning: Unable to convert \"%s\" into a byte value for option %s, using default value of '%c'\n",
          strng, flgName, *defltValue);
        result = ( unsigned char ) *defltValue;
        *flgPtr = FALSE;
      }
    }
#ifdef DEBUG  
    if( D_Flg )  printf( "Debug: The conversion of string \"%s\" for option %s resulted in 0x%02x\n", strng, flgName, result );
#endif
  }
  return( result & BYTE_MASK );
}


void  printOutHelpMessage( char * programName )  {
  printf( "\nUseage:\n" );
  printf( "%s [options] [inputFile1 [inputFile2 [.. inputFileN]]]\n", programName );
  printf( "  where [options] are; -\n" );
  printf( "   -A .. Disable output of ASCII column (bytes in ASCII form normally in 3rd column)\n" );
  printf( "   -b N .. Begin file dumps at an offset (N > 0 offset from start, N < 0 offset from end)\n" );
  printf( "   -B N .. Limit dumps to a maximum of N bytes (where N > 0)\n" );
  printf( "   -c C .. Use printable char C instead of '%c' for a non-printable or non-ASCII char (also overides -A)\n",
    DEFAULT_NON_PRINTABLE_ASCII_INDICATOR_CHAR );
  printf( "   -C .. Cryptogram mode output enable - i.e. shortcut for -I -H -w32 -v6\n" );
  printf( "   -d .. Decimal index output enable & default to 10 bytes per line\n" );
  printf( "   -D .. Debug output enable\n" );
  printf( "   -f N .. Set hex field separator to N character spaces (where %d <= N <= %d)\n",
    MIN_HEX_FIELD_SEPARATOR_WIDTH, MAX_HEX_FIELD_SEPARATOR_WIDTH );
  printf( "   -h .. Print out this help message and exit\n" );
  printf( "   -H .. Disable output of Hexadecimal column (bytes in Hexadecimal form normally in 2nd column)\n" );
  printf( "   -I .. Disable output of Index column (Index number of first byte in a line, normally in 1st column)\n" );
  printf( "   -L N .. Limit dumps to a maximum of N lines (where N > 0)\n" );
  printf( "   -o outfileName .. Specify an output file instead of sending output to stdout\n" );
  printf( "   -s .. Classify space char as printable in Ascii output\n" );
  printf( "   -S C .. Use char C instead of default '%c' char as column separator (where C > '\\0' i.e. not NUL)\n",
    DEFAULT_COLUMN_SEPARATOR_CHAR );
  printf( "   -v[N] .. Verbose output enable, optionally set level to N (where %d <= N <= %d)\n",
    MIN_VERBOSITY_LEVEL, MAX_VERBOSITY_LEVEL );
  printf( "   -w N .. Set bytes processed per line to N (where %d < N <= %d)\n\n",
    MIN_BYTES_PER_LINE - 1, MAX_BYTES_PER_LINE );
  printf( "  where  [inputFile1 [inputFile2 [.. inputFileN]]]\n" );
  printf( "    are optional file name(s) of file(s) to dump. If no input files are specified\n" );
  printf( "    or the file name specified is '-' then input is taken from stdin.\n\n" );
  printf( "Note that if ihad output isn't to your liking you can try; -\n" );
  printf( "  od -A x -t x1z -v inputFile  or  xxd -g 0 inputFile  or  hexdump -C inputFile\n\n" );
}


void  printOptionStatusIfRequired( int  optionFlg, char *  optionName )  {
  if( D_Flg )  printf( "Debug: Option %s is %s (%d)\n", optionName, optionFlg ? "On" : "Off", optionFlg );
}


int  processCommandLineOptions( int  argc, char *  argv[] )  {
  int  result;
  int  ignored;	/* dummy variable */
  extern char *  optarg;
  extern int  optind, opterr, optopt;

/* Isolate the name of the executable */
  exeName = argv[0];	/* set global variable to default */
  if(( exePath = strdup( argv[0] )) == NULL )
    perror( "Warning: Unable to create duplicate of path to this executable" );
  else if(( exeName = basename( exePath )) == NULL )  {
    perror( "Warning: Unable to obtain the name of this executable" );
    exeName = argv[0];	/* set back to the default again */
  }
/* Set all the global flags from command line options */
  opterr = 0;	/* Suppress error messages from getopt() to stderr */
/* Process switch options from the command line */
  while(( result = getopt( argc, argv, optionStr )) != -1 )  {
    switch( result )  {
      case 'A' :  A_Flg = TRUE; break;
      case 'b' :  bFlg = TRUE; bStrng = optarg; break;
      case 'B' :  B_Flg = TRUE; B_Strng = optarg; break;
      case 'c' :  cFlg = TRUE; cStrng = optarg; break;
      case 'C' :  C_Flg = TRUE; break;
      case 'd' :  dFlg = TRUE; break;
      case 'D' :  D_Flg = TRUE; break;
      case 'f' :  fFlg = TRUE; fStrng = optarg; break;
      case 'h' :  hFlg = TRUE; break;
      case 'H' :  H_Flg = TRUE; break;
      case 'I' :  I_Flg = TRUE; break;
      case 'L' :  L_Flg = TRUE; L_Strng = optarg; break;
      case 'o' :  oFlg = TRUE; oStrng = optarg; break;
      case 's' :  sFlg = TRUE; lowestPrintableAsciiChar = ' '; break;
      case 'S' :  S_Flg = TRUE; S_Strng = optarg; break;
      case 'v' :  vFlg = TRUE; vStrng = optarg; break;
      case 'w' :  wFlg = TRUE; wStrng = optarg; break;
      default :
        printf( "Warning: command line option -%c is unrecognised or incomplete and has been ignored\n", optopt );
        printf( "Information: For help on command line options run \"%s -h\"\n", exeName );
        break;
    }
  }

/* Postprocess -D (debug) switch option */
  printOptionStatusIfRequired( D_Flg, "-D" );
  
/* Postprocess -v (verbosityLevel) switch option */
  printOptionStatusIfRequired( vFlg, "-v" );
/* Don't turn off vFlg even if level value is nonsense */
  if( vFlg )  verbosityLevel = convertOptionStringToInteger( MIN_VERBOSITY_LEVEL, vStrng, "-v", &ignored );
  else  {
    verbosityLevel = MIN_VERBOSITY_LEVEL;
#ifdef DEBUG
    if( D_Flg )  printf( "Debug: Option -v was not found in the command line options\n" );
#endif
  }
/* Ensure the verbosity level is in the expected range */
  verbosityLevel = limitIntegerValueToEqualOrWithinRange( verbosityLevel, MIN_VERBOSITY_LEVEL, MAX_VERBOSITY_LEVEL );
  if( D_Flg )  {
    printf( "Debug: verbosity level is %d\n", verbosityLevel );
    if( ! vFlg )  {
      vFlg = TRUE;
      printf( "Warning: verbose option -v set active by -D option\n" );
      printOptionStatusIfRequired( vFlg, "-v" );
    }
  }
  
/* Postprocess -b (begin at offset) switch option */
  printOptionStatusIfRequired( bFlg, "-b" );
  if( bFlg )  beginOffset = convertOptionStringToLong( 0L, bStrng, "-b", &bFlg );
  else  {
#ifdef DEBUG
    if( D_Flg )  printf( "Debug: Option -b was not found in the command line options\n" );
#endif
    beginOffset = 0L;	/* Ensure beginOffset is set to default of start of file */
  }
  if( D_Flg )  printf( "Debug: Begin at an offset in the file of %ld bytes\n", beginOffset );

/* Postprocess -B (dump at most X bytes) switch option */
  printOptionStatusIfRequired( B_Flg, "-B" );
  if( B_Flg )  bytesToDump = convertOptionStringToLong( LONG_MAX, B_Strng, "-B", &B_Flg );
  else  {
#ifdef DEBUG
    if( D_Flg )  printf( "Debug: Option -B was not found in the command line options\n" );
#endif
    bytesToDump = LONG_MAX;	/* Ensure bytesToDump is set to default */
  }
  bytesToDump = limitLongValueToEqualOrMorePositiveThan( bytesToDump, 1L );
  if( D_Flg )  {
    if( B_Flg )  printf( "Debug: Limit bytes to Dump from the file to a max of %ld bytes\n", bytesToDump );
    else  printf( "Debug: Number of bytes to Dump from the file is not limited by -B option\n" );
  }

/* Postprocess -c switch option substitute for a non-printable or non-ASCII char */
  printOptionStatusIfRequired( cFlg, "-c" );
  if( cFlg )  *cCharStrng = ( char ) convertOptionStringToByteChar( &cFlg, cCharStrng, cStrng, "-c" );
  else  {
#ifdef DEBUG
    if( D_Flg )  printf( "Debug: Option -c alternateChar was not found in the command line options\n" );
#endif
  }
  if( cFlg && A_Flg )  {
    A_Flg = FALSE;
    printf( "Warning: -A option reset as a valid -c option overides it\n" );
  }
  if( ! isprint(( int ) *cCharStrng ))  {
    *cCharStrng = DEFAULT_NON_PRINTABLE_ASCII_INDICATOR_CHAR;
    printf( "Warning: -c C option only allows printable chars, defaulting to '%c' as non-printable indicator\n",
      *cCharStrng );
  }
  
/* Postprocess -d (decimal index) switch option */
  printOptionStatusIfRequired( dFlg, "-d" );
#ifdef DEBUG
  if( D_Flg && ( ! dFlg ))  printf( "Debug: Option -d was not found in the command line options\n" );
#endif

/* Postprocess -f (fieldSeparatorWidth) switch option */
  printOptionStatusIfRequired( fFlg, "-f" );
  if( fFlg )  fieldSeparatorWidth = convertOptionStringToInteger( MIN_HEX_FIELD_SEPARATOR_WIDTH, fStrng, "-f", &fFlg );
  else  {
#ifdef DEBUG
    if( D_Flg )  printf( "Debug: Option -f was not found in the command line options\n" );
#endif
  }
/* Ensure the field separator width is in the expected range */
  fieldSeparatorWidth = limitIntegerValueToEqualOrWithinRange( fieldSeparatorWidth, MIN_HEX_FIELD_SEPARATOR_WIDTH, MAX_HEX_FIELD_SEPARATOR_WIDTH );
  if( D_Flg )  printf( "Debug: field separator width is %d\n", fieldSeparatorWidth );

/* Report on -h ( help ) switch option */
  printOptionStatusIfRequired( hFlg, "-h" );
  if( ! hFlg )  {
#ifdef DEBUG
    if( D_Flg )  printf( "Debug: Option -h was not found in the command line options\n" );
#endif
  }

/* Postprocess -o switch option */
  printOptionStatusIfRequired( oFlg, "-o" );
  if( oFlg )  {
    if( oStrng == ( char * ) NULL )  {
      printf( "Warning: File name string for option -o outfileName is uninitialised\n" );
      oFlg = FALSE;
      ofp = stdout;
      fprintf( stderr, "Warning: Defaulting to writing output to stdout\n" );
    }
    else  {
      if( D_Flg )  printf( "Debug: File name string part of option -o is \"%s\"\n", oStrng );
    }
  }
  else  {
#ifdef DEBUG
    if( D_Flg )  printf( "Debug: Option -o outfileName was not found in the command line options\n" );
#endif
  }

/* Postprocess -S switch option char C instead of default space (SP) char as column Separator char */
  printOptionStatusIfRequired( S_Flg, "-S" );
  if( S_Flg )  *S_ColumnChar = ( char ) convertOptionStringToByteChar( &S_Flg, S_ColumnChar, S_Strng, "-S" );
  else  {
#ifdef DEBUG
    if( D_Flg )  printf( "Debug: Option -S char was not found in the command line options\n" );
#endif
  }
  if( S_Flg && D_Flg )
    printf( "Debug: Column separator is '%c' (1st part of option -S is \"%s\")\n", *S_ColumnChar, S_Strng );
/* Note that the Column Separator may not be '\0' i.e. ASCII NUL as it is also the end of string delimiter in C code */
  if( *S_ColumnChar == '\0' )  {
    *S_ColumnChar = DEFAULT_COLUMN_SEPARATOR_CHAR;	/* Reset Column Separator to the default space char */
    printf( "Warning: -S NUL option not allowed, defaulting to '%c' char for Column Separator\n", *S_ColumnChar );
  }

/* Postprocess -w (width) switch option */
  printOptionStatusIfRequired( wFlg, "-w" );
  if( wFlg )  byteDisplayWidth = convertOptionStringToInteger( DEFAULT_BYTES_PER_LINE, wStrng, "-w", &wFlg );
  else  {
#ifdef DEBUG
    if( D_Flg )  printf( "Debug: Option -w was not found in the command line options\n" );
#endif
    byteDisplayWidth = (( dFlg ) ? DEFAULT_DECIMAL_BYTES_PER_LINE : DEFAULT_BYTES_PER_LINE );
  }
/* Ensure the display width is not negetive, not zero and not bigger than MAX_BYTES_PER_LINE */
  byteDisplayWidth = limitIntegerValueToEqualOrWithinRange( byteDisplayWidth, MIN_BYTES_PER_LINE, MAX_BYTES_PER_LINE );
  if( D_Flg )  printf( "Debug: Hex and ASCII column Width is %d bytes\n", byteDisplayWidth );

/* Postprocess -C (cryptogram mode) switch option - must be after -w processing */
  printOptionStatusIfRequired( C_Flg, "-C" );
  if( C_Flg )  {
    I_Flg = TRUE;	/* don't print index */
    H_Flg = TRUE;	/* don't print hex */
    if( ! wFlg )  {	/* if user hasn't set the width then make it the max width */
      wFlg = TRUE;
      byteDisplayWidth = MAX_BYTES_PER_LINE;
    }
    if( ! vFlg )  {	/* if user hasn't set the verbosity then make it 6 */
      vFlg = TRUE;
      verbosityLevel = 6;
    }
  }
#ifdef DEBUG
  if( D_Flg && ( ! C_Flg ))  printf( "Debug: Option -C was not found in the command line options\n" );
#endif

/* Postprocess -L (dump at most X lines) switch option - must be after both -B & -w processing */
  printOptionStatusIfRequired( L_Flg, "-L" );
  if( L_Flg )  {
    linesToDump = convertOptionStringToLong( LONG_MAX / byteDisplayWidth, L_Strng, "-L", &L_Flg);
    linesToDump = limitLongValueToEqualOrWithinRange( linesToDump, 1L, LONG_MAX / byteDisplayWidth );
    if( L_Flg && ( bytesToDump > byteDisplayWidth * linesToDump ))  {	/* Take smaller of max number of lines * width and max number of bytes */
      bytesToDump = byteDisplayWidth * linesToDump;	/* Convert from max number of lines to max number of bytes */
      B_Flg = TRUE;
    }
  }
  else  {
#ifdef DEBUG
    if( D_Flg )  printf( "Debug: Option -L was not found in the command line options\n" );
#endif
    linesToDump = LONG_MAX / byteDisplayWidth;	/* Ensure linesToDump is set to default */
  }
  linesToDump = limitLongValueToEqualOrWithinRange( linesToDump, 1L, LONG_MAX / byteDisplayWidth );
  if( D_Flg )  {
    if( L_Flg )  printf( "Debug: Limit lines to Dump from the file to a max of %ld lines\n", linesToDump );
    else  printf( "Debug: Number of lines to Dump from the file is not limited by -L option\n" );
  }

/* Report on -A ( ASCII column suppress) switch option */
  printOptionStatusIfRequired( A_Flg, "-A" );

/* Report on -H ( Hex column suppress) switch option */
  printOptionStatusIfRequired( H_Flg, "-H" );

/* Report on -I ( Index column suppress) switch option */
  printOptionStatusIfRequired( I_Flg, "-I" );

/* return the index of the first positional argument (i.e. input file name?) */
  return( optind );
}


long  readByteStreamAndPrintIndexHexAscii( FILE *  fp, FILE *  ofp, long startOffset, long  bytesToDumpCnt )  {
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
  int  outputStringSize;	/* must be big enough for MAX_BYTES_PER_LINE bytes per line */
  char *  bPtr;     /* byte pointer */	/* output line space > 8+2+3*MAX_BYTES_PER_LINE+1+MAX_BYTES_PER_LINE+1 */
  char *  aPtr;     /* ascii pointer */
  int  hexFldWdth;
  char  hexFldFrmt[ 11 ];

/* Set or reset flag according to the need to print summary information */
  collectSummary = ( D_Flg || ( verbosityLevel > 1 ) || C_Flg );
  for( byte = 0; byte < 256; byte++ )  freqArray[ byte ] = 0L;
/* Setup the format for printing the hex bytes */
  hexFldWdth = HEX_BYTE_FIELD_WIDTH + fieldSeparatorWidth;
  if( hexFldWdth > ( sizeof( hexFldFrmt ) - 7))  hexFldWdth = ( sizeof( hexFldFrmt ) - 7);
  else if( hexFldWdth < 2 )  hexFldWdth = 2;
/* Note that the Column Separator may not be '\0' i.e. ASCII NUL as it is also the end of string delimiter in C code */
  if( *S_ColumnChar == '\0' )  *S_ColumnChar = DEFAULT_COLUMN_SEPARATOR_CHAR;	/* Silently reset Column Separator to the default char */
  sprintf( hexFldFrmt, "%c02x%c%c%c%c%c",
    '%', *S_ColumnChar, *S_ColumnChar, *S_ColumnChar, *S_ColumnChar, *S_ColumnChar );	/* Set up a string of the form "%02x  " */
  bPtr = hexFldFrmt + ( 2 + hexFldWdth );	/* Temp use of bPtr to point to end of Hex format string */
  *bPtr = '\0';	/* Terminate Hex format string with the correct number of column separators (Defaults to space) */
/* Get a suitably sized buffer to hold the output string */
  outputStringSize = (( I_Flg ) ? 4 : 14 ) + ( hexFldWdth + (( A_Flg ) ? 0 : 1 )) * byteDisplayWidth;
  outputString = malloc( outputStringSize );
  if( outputString == NULL )  {
    fprintf( stderr, "PANIC: malloc( %d ) failed\n", outputStringSize );
    perror( "readByteStreamAndPrintIndexHexAscii()" ); 
  }
  else  {
#ifdef DEBUG
    if( D_Flg )  {
      fprintf( ofp, "Debug: hexFldWdth is %d and hexFldFrmt is \"%s\"\n", hexFldWdth, hexFldFrmt ); 
      fprintf( ofp, "Debug: Size of byteAddr (unsigned int) is %lu bytes\n", sizeof( byteAddr ));
      fprintf( ofp, "Debug: Capacity of output string is %d characters\n", outputStringSize );
    }
#endif
    *outputString = '\0';		/* in-case the stdin or file has 0 length */
 /* Silently ensure the char indicating a non-ascii or non-printable ascii byte is itself printable */
    if( ! isprint(( int ) *cCharStrng ))  *cCharStrng = DEFAULT_NON_PRINTABLE_ASCII_INDICATOR_CHAR;
 /* Take any fseek() skip into account if there was one */
    byteAddr = startOffset;
 /* Read bytes from stdin or file until end of file or if -B or -L options are active the bytes to dump count reaches zero */
    while((( byte = fgetc( fp )) != EOF ) && (( ! B_Flg ) || ( bytesToDumpCnt-- > 0L ))) {
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
        bPtr = ( char * ) memset( outputString, *S_ColumnChar, outputStringSize );
     /* Put the Index in the line buffer, if not disabled */
        if( ! I_Flg )  {
          sprintf( bPtr, ( dFlg ) ? "%08lu%c" : "%08lx%c", byteAddr, *S_ColumnChar );
	      if( fieldSeparatorWidth > 0 )  sprintf( bPtr + strlen( outputString ), "%c", *S_ColumnChar );
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
        else  *aPtr =  *cCharStrng;	/* Print a full stop or alternate printable char instead of non-printable ascii */
        aPtr += 1;
      }
   /* Output current line if required */
      if(( byteCnt % byteDisplayWidth ) == 0 )  {
        if( A_Flg )  sprintf( bPtr, "\n" );	/* terminate line that doesn't have Ascii */
        else  {
          if( ! H_Flg )  *bPtr = *S_ColumnChar;	/* Don't use byte pointer if Hexadecimal is not being output */
          sprintf( aPtr, "\n" );	/* terminate line that does have Ascii */
        }
        if( ! ( I_Flg && H_Flg && A_Flg ))  {	/* Don't print lines of column separators if all 3 columns are off */
          fprintf( ofp, "%s", outputString );	/* print line to output file or stdout */
#ifdef DEBUG
          if( D_Flg )
            fprintf( ofp, "Debug: Output string is %lu characters\n", strlen( outputString ));
#endif
        }
        *outputString = '\0';		/* set string back to zero length */
        byteAddr = startOffset + byteCnt;
      }
    }
 /* Finish off last line if it is a short line */
    if( strlen( outputString ) > 0 )  {
      if( A_Flg )  sprintf( bPtr, "\n" );	/* terminate line that doesn't have Ascii */
      else  {
        if( ! H_Flg )  *bPtr = *S_ColumnChar;	/* Don't use byte pointer if Hexadecimal is not being output */
        sprintf( aPtr, "\n" );	/* terminate line that does have Ascii */
      }
      if( ! ( I_Flg && H_Flg && A_Flg ))  {	/* Don't print last line of column separators if all 3 columns are off */
        fprintf( ofp, "%s", outputString );	/* print line to output file or stdout */
#ifdef DEBUG
        if( D_Flg )
          fprintf( ofp, "Debug: Output string is %lu characters\n", strlen( outputString ));
#endif
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
  int  returnVal;	/* this code returns 0 for success */
  long  byteCnt;
  FILE *  fp;
  long  fileSize;
  long  fileOffset;

#ifdef DEBUG
  if( D_Flg )
    fprintf( ofp, "Debug: Executing: processA_SingleCommandLineParameter( %s )\n", nameStrng );
#endif
  returnVal = 0;
/* Keep beginOffset global variable safe from modification in case there are multiple files */
  fileOffset = beginOffset;
/* Open the file for reading (in binary mode) */
  returnVal = (( fp = fopen( nameStrng, "rb" ) ) == NULL );
  if( returnVal )  {
    fprintf( ofp, "Warning: Unable to open a file named \"%s\" for reading\n", nameStrng );
    perror( nameStrng );
  }
  else  {	/* Begin file opened ok block */
 /* obtain file size for the file that was just opened */
    result = fseek( fp, 0L, SEEK_END );
    if( result != 0 )  {
      fileSize = LONG_MIN;
      fprintf( ofp, "Warning: Unable to determine file size by seeking to the end of the file named \"%s\"\n", nameStrng );
      perror( nameStrng );
   /* If there was a problem with the seek make sure it starts at 0 */
      rewind( fp );
      fileOffset = 0L;
    }
    else  {	/* Begin first fseek successful block */
      fileSize = ( long ) ftell( fp );
   /* Ensure file is reset back to starting at 0 */
      rewind( fp );
      if( vFlg )  fprintf( ofp, "File: \"%s\" is %ld bytes\n", nameStrng, fileSize );
      if( fileSize == 0L )  {	/* Is there any bytes in the file to process? */
        if( vFlg )  fprintf( ofp, "Warning: There are no bytes to dump in \"%s\"\n", nameStrng );
      }
      else if( fileSize < 0L )  {
        fprintf( ofp, "Warning: There are less than zero (%ld) bytes to dump in \"%s\" - trying anyway\n", fileSize, nameStrng );
      }
      else  {	/* Begin file size greater than zero block, so process the file */
        if( D_Flg )
          fprintf( ofp, "Debug: The size of \"%s\" is %ld bytes\n", nameStrng, fileSize );
     /* If begin option has been set then seek to the new start */
        if( bFlg )  {
          if( fileOffset > 0L )  {
         /* If fileSize is valid then make sure the seek offset isn't bigger than the file */
            if(( fileSize > 0L ) && ( fileOffset > fileSize ))  fileOffset = fileSize;
         /* Set up the start of the dumping at the begin offset */
            result = fseek( fp, fileOffset, SEEK_SET );
#ifdef DEBUG
            if( D_Flg )
              fprintf( ofp, "Debug: result of fseek() from start of file was %d\n", result );
#endif
          }
          else if( fileOffset < 0L )  {
         /* If fileSize is valid then make sure the seek offset isn't bigger than the file */
            if(( fileOffset + fileSize ) < 0L )  fileOffset = -fileSize;
            if( D_Flg )  fprintf( ofp, "Debug: fileOffset for SEEK_END is %ld\n", fileOffset );
         /* Set up to start dumping at the begin offset from the end of the file */
            result = fseek( fp, fileOffset, SEEK_END );
         /* Adjust fileOffset to print correct Index */
            if( result == 0 )  fileOffset += fileSize;
#ifdef DEBUG
            if( D_Flg )
              fprintf( ofp, "Debug: result of fseek() from end of file was %d\n", result );
#endif
          }
        }
      }	/* End of file size is greater than 0 block */
    }	/* End first fseek successful block */
 /* Process the file opened near the start of this block of code */
    byteCnt = readByteStreamAndPrintIndexHexAscii( fp, ofp, fileOffset, bytesToDump );
 /* Close the file just processed */
    result = ( fclose( fp ) != 0 );
    if( result )  {
      fprintf( ofp, "Warning: Unable to close the file named \"%s\"\n", nameStrng );
      perror( nameStrng );
    }
    returnVal += result;	/* increment returnVal if fclose() on input file failed */
    if( D_Flg || vFlg )
      fprintf( ofp, "File: \"%s\" (%ld bytes processed)\n", nameStrng, byteCnt );
  }	/* End of file opened ok block */
  return( returnVal );
}


int  processNonSwitchCommandLineParameters( int  frstIndx, int  lstIndx, char *  cmdLnStrngs[] )  {
  int  returnVal;	/* returnVal is incremented for each error */
  int  result, indx;
  int  chrCnt;

#ifdef DEBUG
  if( D_Flg )  {
    printf( "Debug: Executing: processNonSwitchCommandLineParameters()\n" );
    printf( "Debug: first index is %d and last index is %d\n", frstIndx, lstIndx );
    for( indx = frstIndx; indx <= lstIndx; indx++ )
      printf( "Debug: cmdLnStrngs[ %d ] string is \"%s\"\n", indx, cmdLnStrngs[ indx ] );
  }
#endif
  returnVal = (( oFlg ) && (( oStrng == ( char * ) NULL ) || ( *oStrng == '\0' )));
  if( returnVal )
    fprintf( stderr, "Warning: -o specified, but no output file name specified - aborting\n" );
  else  {
  /* Attempt to open the output file if required */
    returnVal = ( oFlg ) ? (( ofp = fopen( oStrng, "w" )) == NULL ) : 0;
    if( returnVal )  {
      printf( "Warning: Unable to open a file named \"%s\" for writing output- aborting\n", oStrng );
      if( D_Flg )  perror( oStrng );
    }
    else  {
      if((( lstIndx + 1 ) == frstIndx ) ||
        (( lstIndx  == frstIndx ) && ( strncmp( cmdLnStrngs[ frstIndx ], "-", 1 ) == 0 )))  {
     /* There are no files specified in the command line or the filename is "-" so process stdin */
        if( D_Flg )
          printf( "Debug: Reading bytes from stdin - mark end of any manually typed input with EOT (i.e. ^D)\n" );
        chrCnt = readByteStreamAndPrintIndexHexAscii( stdin, ofp, 0L, bytesToDump );	/* 0L means no fseek()/skip done on stdin */
        if( D_Flg || vFlg )  printf( "Processed %d chars from stdin\n", chrCnt );
      }
      else  {
        if( D_Flg )
          printf( "Debug: Largest file size that can be safely handled is %ld bytes\n", LONG_MAX );
     /* Process each file specified in the command line */
        for( indx = frstIndx; indx <= lstIndx; indx++ )  {
          result = processA_SingleCommandLineParameter( ofp, cmdLnStrngs[ indx ] );
          returnVal += result;	/* Count the unsuccessful results */
          if( D_Flg )  printf( "Debug: result %d, returnVal %d\n", result, returnVal );
        }
      }
   /* Close output file specified in the command line, if there was one */
      result = ( oFlg ) ? (fclose( ofp ) != 0) : 0;
      if( result )  {
        fprintf( stderr, "Warning: Attempt to close the output file named \"%s\" failed\n", oStrng );
        if( D_Flg )  perror( oStrng );
      }
      returnVal += result;	/* Count any unsuccessful fclose() on output file */
    }
  }
  return( returnVal );
}


int  main( int  argc, char *  argv[] )  {
  int  resultIndex;
#ifdef DEBUG
  int  indx;
#endif

/* Preset switch option Flags and Data */
  setGlobalFlagDefaults( argv );	/* Set up any Global variables not already set at declaration */

/* Process switch options from the command line */
  resultIndex = processCommandLineOptions( argc, argv );

/* If Debug or verbose options are True then print code details */
  if( D_Flg || (( verbosityLevel > 0 ) && ( verbosityLevel < 3 )))  {
    printf( "Source Code Control Id (RCS) %s\n", SRC_CODE_CNTRL_ID );
    print_byteFreq_SourceCodeControlIdentifier( stdout );
    printf( "Source file \"%s\", compiled on %s at %s\n", __FILE__, __DATE__, __TIME__ );
    print_byteFreq_SourceCompileDetails( stdout );
  }

/* If -h switch option used then print help message and exit */
  if( hFlg )  {
    printOutHelpMessage( exeName );
    return( 1 );
  }

/* If Debug then show rest of non-switch command line options if there are any */
#ifdef DEBUG
  if( D_Flg )  {
    printf( "Debug: argc is %d and option index is %d\n", argc, resultIndex );
    for( indx = resultIndex; indx < argc; indx++ )  {
      printf( "Debug: argv[ %d ] string is \"%s\"\n", indx, argv[ indx ] );
    }
  }
#endif
  
/* Attempt to dump Index, Hex and Ascii and return 0 if successful */
  return( processNonSwitchCommandLineParameters( resultIndex, argc - 1, argv ));
}

