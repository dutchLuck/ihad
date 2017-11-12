/*
 * I H A D
 *
 * Index Hex Ascii Dump of a (binary) file or stdin.
 *
 * ihad.c, 0v1, last edited on Fri Nov 25 13:54:17 2016 by OFH.
 *
 * This is not production code! It is a toy, PLEASE do not use it
 * for anything serious! Instead use; -
 *  hexdump -C yourFile
 *  xxd yourFile
 *  od -A x -t x1z -v yourFile
 * OR
 *  http://www.fileformat.info/tool/hexdump.htm 
 *
 * In addition, PLEASE do not look at the source code as an example
 * of how to code!
 * This code exists for my amusement & my education, but you are
 * free to make use of it for any trivial purpose.
 * It provides marginally cleaner output (i.e. just spaces as
 * column / field delimiters) than do the command line
 * utilities listed above.
 */

/*
 * Output a human readable dump of input (bytes) from either stdin or file(s).
 *
 * i.e. read input stream and output lines with the format
 *  index  hex digit 1 .. hex digit N  ascii char 1 .. ascii char N
 *
 * if there is no ascii printable character corresponding to a byte of input
 * then a full stop is printed. By default space is not considered printable.
 *
 * e.g.
 * $ ./ihad tmp.bin
 * 00000000 54686973206973206120737472696e67 This.is.a.string
 * 00000010 206f6620636861726163746572730a   .of.characters.
 * $
 *
 * Output of the index (first column) may be switched off with the -I switch.
 * Output of the ascii (third column) may be switched off with the -A switch.
 *
 * For useage help type the switch -h;
 * e.g.
 *  ./ihad -h
 * 
 */

/*
 * $Log: ihad.c,v $
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
 *
 */

#include <stdio.h>	/* printf() fopen() perror() fgetc() fclose() fprintf() */
#include <stdlib.h>	/* atoi() malloc() free() */
#include <unistd.h>	/* getopt() */
#include <string.h>	/* memset() strlen() */

#define  SRC_CODE_CNTRL_ID  "$Id: ihad.c,v 0.3 2016/11/26 13:12:37 owen Exp dutchLuck $"

#define  BYTE_MASK 0xff
#define  WORD_MASK 0xffff
#define  DEFAULT_WIDTH 16
#define  DEFAULT_DECIMAL_WIDTH 10
#define  MAX_WIDTH 32
#define  HEX_BYTE_FIELD_WIDTH 2		/* Default is 2 which is no spaces */

int  processNonSwitchCommandLineParameters( int  frstIndx, int  lstIndx, char *  cmdLnStrngs[] );
int  processA_SingleCommandLineParameter( char *  nameStrng );

/* Optional Switches: */
/*  Ascii, decimal, Debug, help, Index, outFile, space, verbosity, width */
char  optionStr[] = "AdDhf::Io:sv::w::";
int  A_Flg;			/* Control Ascii column output */
int  dFlg;			/* use decimal index and a default hex width of 10 bytes per line */
int  D_Flg;			/* Control Debug info output */
int  fFlg, fieldSeparatorWidth;	/* Control hex field separation width */
char *  fStrng;
int  hFlg;			/* Control help info output */
int  I_Flg;			/* Control Index column output */
int  oFlg;			/* Control output file output */
char *  oStrng;			/* output file name */
FILE * ofp;			/* output file pointer */
int  sFlg;			/* Control output of space in ascii column */
char  lowestPrintableAsciiChar;
int  vFlg, verbosityLevel;	/* Control verbosity level */
char *  vStrng;
int  wFlg, byteDisplayWidth;	/* Control number of bytes dealt with per line */
char *  wStrng;


void  printOutHelpMessage( char * programName )  {
  printf( "\nUseage:\n" );
  printf( "%s [options] [inputFile1 [inputFile2 [.. inputFileN]]]\n", programName );
  printf( "  where options are '-A -d -D -fX -h -I -o outfileName -s -vX -wX'; -\n" );
  printf( "   -A .. Ascii output disable\n" );
  printf( "   -d .. Decimal index output enable & default to 10 bytes per line\n" );
  printf( "   -D .. Debug output enable\n" );
  printf( "   -fX .. Set hex field separator to X spaces (where 0 < X < 2)\n" );
  printf( "   -h .. Print out this help message and exit\n" );
  printf( "   -I .. Index output disable\n" );
  printf( "   -o outfileName .. Specify an output file instead of sending output to stdout\n" );
  printf( "   -s .. classify space char as printable in Ascii output\n" );
  printf( "   -vX .. Set verbosity level to X (where 0 < X < 4)\n" );
  printf( "   -wX .. Set bytes per line to X (where 0 < X <= %d)\n\n", MAX_WIDTH );
  printf( "  where; -\n" );
  printf( "   [inputFile1 [inputFile2 [.. inputFileN]]]  are optional file name(s)\n" );
  printf( "    of file(s) to dump in hex & ascii\n" );
  printf( "   Note that if no input file is specified then input is taken from stdin.\n\n" );
  printf( "Note that if ihad output isn't acceptable you can try; -\nxxd\nhexdump -C\nod -A x -t x1z -v\n\n" );
}


int  main( int  argc, char *  argv[] )  {
  int  result, indx;
  extern char *  optarg;
  extern int  optind, opterr, optopt;

  opterr = 0;	/* Suppress error messages from getopt() to stderr */

/* Preset command line options */
  A_Flg = 0;			/* Default is Ascii column output */
  dFlg = 0;			/* Default to output hex index rather than decimal index */
  D_Flg = 0;			/* Default to Debug off */
  fFlg = fieldSeparatorWidth = 0;	/* Default to no spaces between hex bytes */
  fStrng = ( char * ) NULL;
  hFlg = 0;			/* Default to no help text output */
  I_Flg = 0;			/* Default is Index column output */
  oFlg = 0;			/* Default to output to stdout */
  oStrng = ( char * ) NULL;
  ofp = stdout;			/* Output file pointer defaults to stdout */
  sFlg = 0;			/* Default to replacing space with a '.' in ascii column */
  lowestPrintableAsciiChar = '!';	/* '!' is next to space */
  vFlg = verbosityLevel = 0;	/* Default to no verbose output */
  vStrng = ( char * ) NULL;
  wFlg = 0;			/* Default to DEFAULT_WIDTH bytes per line */
  byteDisplayWidth = DEFAULT_WIDTH;
  wStrng = ( char * ) NULL;

/* Process switch options from the command line */
  while(( result = getopt( argc, argv, optionStr )) != -1 )  {
    switch( result )  {
      case 'A' :  A_Flg = 1; break;
      case 'd' :  dFlg = 1; break;
      case 'D' :  D_Flg = 1; break;
      case 'f' :  fFlg = 1; fStrng = optarg; break;
      case 'h' :  hFlg = 1; break;
      case 'I' :  I_Flg = 1; break;
      case 'o' :  oFlg = 1; oStrng = optarg; break;
      case 's' :  sFlg = 1; lowestPrintableAsciiChar = ' '; break;
      case 'v' :  vFlg = 1; vStrng = optarg; break;
      case 'w' :  wFlg = 1; wStrng = optarg; break;
      default :
        printf( "?? command line option '-%c' is unrecognised or incomplete and has been ignored\n", optopt );
        printf( " for help on command line options run '%s -h'\n", argv[0] );
        break;
    }
  }

/* Postprocess -D (debug) switch option */
  if( D_Flg )  {
    printf( "Flag for option '-D' is %d\n", D_Flg );
  }

/* Postprocess -v (verbosityLevel) switch option */
  if( D_Flg )  printf( "Flag for option '-v' is %d\n", vFlg );
  if( vFlg )  {
    if( vStrng == ( char * ) NULL )  {
      if( D_Flg )  printf( "String for option '-v' is uninitialised, using default value of 1\n" );
      verbosityLevel = 1;
    }
    else  {
      if( D_Flg )  printf( "String for option '-v' is %s\n", vStrng );
      verbosityLevel = atoi( vStrng );
      if( verbosityLevel > 4 )  verbosityLevel = 4;
      else if( verbosityLevel < 1 )  verbosityLevel = 1; 
    }
    if( D_Flg )  printf( "verbosity level is %d\n", verbosityLevel );
  }
  else  {
    if( D_Flg )  printf( "switch '-v' was not found in the command line options\n" );
  }

  if( D_Flg || ( verbosityLevel > 1 ))  {
    printf( "Source Code Control Id (RCS) %s\n", SRC_CODE_CNTRL_ID );
    printf( "Source file %s, compiled on %s at %s\n", __FILE__, __DATE__, __TIME__ );
  }

  
/* Postprocess -f (fieldSeparatorWidth) switch option */
  if( D_Flg )  printf( "Flag for option '-f' is %d\n", fFlg );
  if( fFlg )  {
    if( fStrng == ( char * ) NULL )  {
      if( D_Flg )  printf( "String for option '-f' is uninitialised, using default value of 1\n" );
      fieldSeparatorWidth = 1;
    }
    else  {
      if( D_Flg )  printf( "String for option '-f' is %s\n", fStrng );
      fieldSeparatorWidth = atoi( fStrng );
      if( fieldSeparatorWidth > 2 )  fieldSeparatorWidth = 2;
      else if( fieldSeparatorWidth < 1 )  fieldSeparatorWidth = 1; 
    }
    if( D_Flg )  printf( "field separator width is %d\n", fieldSeparatorWidth );
  }
  else  {
    if( D_Flg )  printf( "switch '-f' was not found in the command line options\n" );
  }
  
/* Postprocess -w (width) switch option */
  if( D_Flg )  printf( "Flag for option '-w' is %d\n", wFlg );
  if( wFlg )  {
    if( wStrng == ( char * ) NULL )  {
      byteDisplayWidth = DEFAULT_WIDTH;
      if( D_Flg )  printf( "String for option '-w' is uninitialised, using default value of %d\n", byteDisplayWidth );
    }
    else  {
      if( D_Flg )  printf( "String for option '-w' is %s\n", wStrng );
      byteDisplayWidth = atoi( wStrng );
      if( byteDisplayWidth > MAX_WIDTH )  byteDisplayWidth = MAX_WIDTH;
      else if( byteDisplayWidth < 1 )  byteDisplayWidth = DEFAULT_WIDTH; 
    }
  }
  else  {
    if( D_Flg )  printf( "switch '-w' was not found in the command line options\n" );
    byteDisplayWidth = (( dFlg ) ? DEFAULT_DECIMAL_WIDTH : DEFAULT_WIDTH );
  }
  if( D_Flg )  printf( "byte Display Width is %d\n", byteDisplayWidth );
  
/* Postprocess -o switch option */
  if( oFlg )  {
    if( D_Flg )  printf( "Flag for option '-o' is %d\n", oFlg );
    if( oStrng == ( char * ) NULL )  {
      printf( "?? String for option '-o outfileName' is uninitialised\n" );
      oFlg = 0;
      ofp = stdout;
      fprintf( stderr, "Defaulting to writing output to stdout\n" );
    }
    else  {
      if( D_Flg )  printf( "String part of option '-o' is '%s'\n", oStrng );
      result = (( ofp = fopen( oStrng, "w" ) ) != NULL );
      if( ! result )  {
        printf( "?? Unable to open a file named '%s' for writing output\n", oStrng );
        perror( "processA_SingleCommandLineParameter" );
	ofp = stdout;
	fprintf( stderr, "Defaulting to writing output to stdout\n" );
      }
    }
  }
  else  {
    if( D_Flg )  printf( "switch '-o outfileName' was not found in the command line options\n" );
  }

/* Postprocess -h switch option */
  if( hFlg )  {
    if( D_Flg )  printf( "Flag for option '-h' is %d\n", hFlg );
    printOutHelpMessage( argv[0] );
    return 1;
  }
  else  {
    if( D_Flg )  printf( "switch '-h' was not found in the command line options\n" );
  }

/* Process rest of non-switch command line options if there are any */
  if( D_Flg )  {
    printf( "argc is %d and optind is %d\n", argc, optind );
    for( indx = optind; indx < argc; indx++ )  {
      printf( "argv[ %d ] string is '%s'\n", indx, argv[ indx ] );
    }
  }
  processNonSwitchCommandLineParameters( optind, argc - 1, argv );
  return 0;
}


int  readByteStreamAndPrintIndexHexAscii( FILE *  fp )  {
  int  byte;
  int  byteCnt = 0;
  unsigned int  byteAddr = 0;
  char *  outputString;			/* pointer to string to print */
  int  outputStringSize;		/* must be big enough for MAX_WIDTH bytes per line */
  char *  bPtr;	/* byte pointer */	/* output line space > 8+2+3*MAX_WIDTH+1+MAX_WIDTH+1 */
  char *  aPtr; /* ascii pointer */
  int  hexFldWdth;
  char  hexFldFrmt[ 11 ];

/* Setup the format for printing the hex bytes */
  hexFldWdth = HEX_BYTE_FIELD_WIDTH + fieldSeparatorWidth;
  if( hexFldWdth > ( sizeof( hexFldFrmt ) - 7))  hexFldWdth = ( sizeof( hexFldFrmt ) - 7);
  else if( hexFldWdth < 2 )  hexFldWdth = 2;
  sprintf( hexFldFrmt, "%c02x     ", '%' );
  bPtr = hexFldFrmt + ( 2 + hexFldWdth );
  *bPtr = '\0';
/* Get a suitably sized buffer to hold the output string */
  outputStringSize = (( I_Flg ) ? 4 : 14 ) + ( hexFldWdth + (( A_Flg ) ? 0 : 1 )) * byteDisplayWidth;
  outputString = malloc( outputStringSize );
  if( outputString == NULL )  {
    fprintf( stderr, "?? malloc failed\n" );
    perror( "readByteStreamAndPrintIndexHexAscii()" ); 
  }
  else  {
    if( D_Flg )  {
      printf( "hexFldWdth is %d and hexFldFrmt is '%s'\n", hexFldWdth, hexFldFrmt ); 
      printf( "Size of byteAddr (unsigned int) is %lu bytes\n", sizeof( byteAddr ));
      printf( "Capacity of output string is %d characters\n", outputStringSize );
    }
    *outputString = '\0';		/* in-case the stdin or file has 0 length */
 /* Read bytes from stdin or file */
    while(( byte = fgetc( fp )) != EOF )  {
      byte &= BYTE_MASK;
      byteCnt += 1;
   /* Initialize new line if required */
      if(( byteAddr % byteDisplayWidth ) == 0 )  {
        bPtr = ( char * ) memset( outputString, 0x20, outputStringSize );
        if( ! I_Flg )  {
          sprintf( bPtr, ( dFlg ) ? "%08u " : "%08x ", byteAddr );
	  if( fieldSeparatorWidth > 0 )  sprintf( bPtr + strlen( outputString ), " " );
          bPtr += strlen( outputString );
        }
        aPtr = bPtr + hexFldWdth * byteDisplayWidth + 1;
      }
   /* Put byte into outputString in hex form */
      sprintf( bPtr, hexFldFrmt, byte );		/* Print byte as hex number */
      bPtr += hexFldWdth;
   /* If Ascii output is required put byte into outputString in char form */
      if( ! A_Flg )  {
        if(( byte >= lowestPrintableAsciiChar ) && ( byte <= 0x7e ))	/* Test for printable Ascii */
          sprintf( aPtr, "%c", byte );		/* Print byte as ascii character */
        else  sprintf( aPtr, "." );		/* Print a full stop instead of non-printable ascii */
        aPtr += 1;
      }
   /* Output current line if required */
      if(( ++byteAddr % byteDisplayWidth ) == 0 )  {
        if( A_Flg )  sprintf( bPtr, "\n" );	/* terminate line that doesn't have Ascii */
        else  {
          *bPtr = ' ';
          sprintf( aPtr, "\n" );	/* terminate line that does have Ascii */
        }
        fprintf( ofp, "%s", outputString );	/* print line to output file or stdout */
        if( D_Flg )
          printf( "Output string is %lu characters\n", strlen( outputString ));
        *outputString = '\0';		/* set string back to zero length */
      }
    }
 /* Finish off last line if it is a short line */
    if( strlen( outputString ) > 0 )  {
      if( A_Flg )  sprintf( bPtr, "\n" );	/* terminate line that doesn't have Ascii */
      else  {
        *bPtr = ' ';
        sprintf( aPtr, "\n" );	/* terminate line that does have Ascii */
      }
      fprintf( ofp, "%s", outputString );	/* print line to output file or stdout */
      if( D_Flg )
        printf( "Output string is %lu characters\n", strlen( outputString ));
    }
    if( outputString != NULL )  {
      free( outputString );
    }
  }
  return( byteCnt );
}


int  processA_SingleCommandLineParameter( char *  nameStrng )  {
  int  result, byteCnt;
  FILE *  fp;

  if( D_Flg || ( verbosityLevel > 1 ))  {
    printf( "Executing: processA_SingleCommandLineParameter( %s )\n", nameStrng );
  }
  result = (( fp = fopen( nameStrng, "r" ) ) != NULL );
  if( ! result )  {
    printf( "?? Unable to open a file named '%s' for reading\n", nameStrng );
    perror( "processA_SingleCommandLineParameter()" );
  }
  else  {
    byteCnt = readByteStreamAndPrintIndexHexAscii( fp );
    result = ( fclose( fp ) == 0 );
    if( ! result )  {
      printf( "?? Unable to close the file named '%s'\n", nameStrng );
      perror( "processA_SingleCommandLineParameter()" );
    }
    if( D_Flg || vFlg )  printf( "File %s is %d bytes\n", nameStrng, byteCnt );
  }
  return( result );
}


int  processNonSwitchCommandLineParameters( int  frstIndx, int  lstIndx, char *  cmdLnStrngs[] )  {
  int  result, indx;
  int  chrCnt;

  result = 0;
  if( D_Flg )  {
    printf( "Executing: processNonSwitchCommandLineParameters()\n" );
    printf( "first index is %d and last index is %d\n", frstIndx, lstIndx );
    for( indx = frstIndx; indx <= lstIndx; indx++ )  {
      printf( "cmdLnStrngs[ %d ] string is '%s'\n", indx, cmdLnStrngs[ indx ] );
    }
  }
  if(( lstIndx + 1 ) == frstIndx )  {
    chrCnt = readByteStreamAndPrintIndexHexAscii( stdin );
    if( D_Flg || vFlg )  printf( "Processed %d chars from stdin\n", chrCnt );
  }
  else  {
    for( indx = frstIndx; indx <= lstIndx; indx++ )  {
      result = processA_SingleCommandLineParameter( cmdLnStrngs[ indx ] );
    }
  }
  return( result );
}

