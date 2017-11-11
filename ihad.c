/*
 * H A D
 *
 * had.c, 0v0, last edited on Wed Nov  9 00:00:27 2016 by OFH.
 *
 */

/*
 * Read stdin or a file or files and print lines with the format
 *  index  hex  ascii
 * as a representation of the input or contents of the file(s).
 *
 */

/*
 * $Log: ihad.c,v $
 * Revision 0.0  2016/11/12 12:05:18  owen
 * Outputs dump to stdout with adjustable width & index & ascii
 * can be suppressed. Doesn't implement all defined switches yet.
 *
 *
 * Revision 0.0  2015/10/20 01:04:17  owen
 * Original Code from scratch.
 *
 *
 */

#include <stdio.h>	/* printf() fopen() perror() fgetc() fclose() fprintf() */
#include <stdlib.h>	/* atoi() */
#include <unistd.h>	/* getopt() */
#include <string.h>	/* memset() */

#define  SRC_CODE_CNTRL_ID  "$Id: ihad.c,v 0.0 2016/11/12 12:05:18 owen Exp $"

#define  BYTE_MASK 0xff
#define  WORD_MASK 0xffff
#define  DEFAULT_WIDTH 16
#define  DEFAULT_DECIMAL_WIDTH 10
#define  MAX_WIDTH 64

int  processNonSwitchCommandLineParameters( int  frstIndx, int  lstIndx, char *  cmdLnStrngs[] );
int  processA_SingleCommandLineParameter( char *  nameStrng );

char  optionStr[] = "AdDf:hIv::w::";	/* Ascii, decimal, Debug, file, help, Index, verbosity options, width */
int  A_Flg;			/* Control Ascii column output */
int  dFlg;			/* use decimal index and a default hex width of 10 */
int  D_Flg;			/* Control Debug info output */
int  fFlg;
char *  fStrng;
int  hFlg;
int  I_Flg;			/* Control Index column output */
int  vFlg, verbosityLevel;
char *  vStrng;
int  wFlg, byteDisplayWidth;
char *  wStrng;


void  printOutHelpMessage( char * programName )  {
  printf( "\nUseage:\n" );
  printf( "%s -A -d -D -f fileName -h -I -vX -wX inputFile1 inputFile2 .. inputFileN\n", programName );
  printf( "  where; -\n" );
  printf( "   -A .. Ascii output disable\n" );
  printf( "   -d .. Decimal index output enable\n" );
  printf( "   -D .. Debug output enable\n" );
  printf( "   -f fileName .. Specify a file\n" );
  printf( "   -h .. Print out this help message\n" );
  printf( "   -I .. Index output disable\n" );
  printf( "   -vX .. Set verbosity level to X (where 0 < X < 4)\n" );
  printf( "   -wX .. Set bytes per line to X (where 0 < X <= %d)\n", MAX_WIDTH );
  printf( "   inputFile1 inputFile2 .. inputFileN  file name(s) of files to dump \n\n" );
}


int  main( int  argc, char *  argv[] )  {
  int  result, indx;
  extern char *  optarg;
  extern int  optind, opterr, optopt;

  opterr = 0;	/* Suppress error messages from getopt() to stderr */

/* Preset command line options */
  A_Flg = 0;
  dFlg = 0;
  D_Flg = 0;
  fFlg = 0;
  fStrng = ( char * ) NULL;
  hFlg = 0;
  vFlg = verbosityLevel = 0; 
  vStrng = ( char * ) NULL;
  wFlg = 0;
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
      case 'v' :  vFlg = 1; vStrng = optarg; break;
      case 'w' :  wFlg = 1; wStrng = optarg; break;
      default :
        printf( "?? command line option -%c is unrecognised or incomplete and has been ignored\n", optopt );
        printf( " for command line option help run '%s -h'\n", argv[0] );
        break;
    }
  }

/* Postprocess -D (debug) switch option */
  if( D_Flg )  {
    printf( "Flag for option '-D' is %d\n", D_Flg );
    printf( "Source Code Control Id (RCS) %s\n", SRC_CODE_CNTRL_ID );
    printf( "Source file %s, compiled on %s at %s\n", __FILE__, __DATE__, __TIME__ );
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
      else if( byteDisplayWidth < 1 )  byteDisplayWidth = 1; 
    }
  }
  else  {
    if( D_Flg )  printf( "switch '-w' was not found in the command line options\n" );
    byteDisplayWidth = (( dFlg ) ? DEFAULT_DECIMAL_WIDTH : DEFAULT_WIDTH );
  }
  if( D_Flg )  printf( "byte Display Width is %d\n", byteDisplayWidth );
  
/* Postprocess -f switch option */
  if( fFlg )  {
    if( D_Flg )  printf( "Flag for option '-f' is %d\n", fFlg );
    if( fStrng == ( char * ) NULL )  {
      printf( "?? String for option '-f fileName' is uninitialised\n" );
      fFlg = 0;
    }
    else  {
      if( D_Flg )  printf( "String part of option '-f' is '%s'\n", fStrng );
    }
  }
  else  {
    if( D_Flg )  printf( "switch '-f fileName' was not found in the command line options\n" );
  }

/* Postprocess -h switch option */
  if( hFlg )  {
    if( D_Flg )  printf( "Flag for option '-h' is %d\n", hFlg );
    printOutHelpMessage( argv[0] );
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
  int  byteAddr = 0;
  char  outputString[ 512 ];
  char *  bPtr;
  char *  aPtr;

  *outputString = '\0';		/* in-case the stdin or file has 0 length */
/* Read info from stdin or file */
  while(( byte = fgetc( fp )) != EOF )  {
    byte &= BYTE_MASK;
    byteCnt += 1;
 /* Initialize new line if required */
    if(( byteAddr % byteDisplayWidth ) == 0 )  {
      bPtr = ( char * ) memset( outputString, 0x20, sizeof( outputString ));
      if( ! I_Flg )  {
        sprintf( bPtr, ( dFlg ) ? "%08d  " : "%08x  ", byteAddr );
        bPtr += strlen( outputString );
      }
      aPtr = bPtr + 3 * byteDisplayWidth + 1;
    }
    sprintf( bPtr, "%02x ", byte );
    bPtr += 3;
    if( ! A_Flg )  {
      if(( byte >= 0x20 ) && ( byte <= 0x7e ))  {
        sprintf( aPtr, "%c", byte );
      }
      else  sprintf( aPtr, "." );
      aPtr += 1;
    }
 /* Output current line if required */
    if(( ++byteAddr % byteDisplayWidth ) == 0 )  {
      if( A_Flg )  {
        sprintf( bPtr, "\n" );	/* terminate line that doesn't have Ascii */
      }
      else  {
        *bPtr = ' ';
        sprintf( aPtr, "\n" );	/* terminate line that does have Ascii */
      }
      printf( "%s", outputString );	/* print line to stdout */
      *outputString = '\0';		/* set string back to zero length */
    }
  }
 /* Finish off last line if it is a short line */
  if( strlen( outputString ) > 0 )  {
    if( A_Flg )  {
      sprintf( bPtr, "\n" );	/* terminate line that doesn't have Ascii */
    }
    else  {
      *bPtr = ' ';
      sprintf( aPtr, "\n" );	/* terminate line that does have Ascii */
    }
    printf( "%s", outputString );	/* print line to stdout */
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
    perror( "processA_SingleCommandLineParameter" );
  }
  else  {
    byteCnt = readByteStreamAndPrintIndexHexAscii( fp );
    result = ( fclose( fp ) == 0 );
    if( ! result )  {
      printf( "?? Unable to close the file named '%s'\n", nameStrng );
      perror( "processA_SingleCommandLineParameter" );
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

