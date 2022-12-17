# ihad
This utility program is named "ihad" because it provides "index" "hex" "ascii" "dump"
of bytes (8 bit data) sent to it's input or obtained from one or more files.
By default "ihad" dumps stdin input or file data in a format of multiple
lines of three columns separated by a space character to stdout output.
The first column is an index number representing an offset from the start
of the input/file to the leftmost data byte in the second and third columns.
The second column is a hexadecimal representation of the value of each byte in
the stream of input data and the third column shows any ASCII characters in
those same bytes of data.
An example of the default output of ihad is; -

```
$ ./ihad tmp.dat
00000000 000102030405060708090a0b0c0d0e0f ................
00000010 101112131415161718191a1b1c1d1e1f ................
00000020 202122232425262728292a2b2c2d2e2f .!"#$%&'()*+,-./
00000030 303132333435363738393a3b3c3d3e3f 0123456789:;<=>?
00000040 404142434445464748494a4b4c4d4e4f @ABCDEFGHIJKLMNO
00000050 505152535455565758595a5b5c5d5e5f PQRSTUVWXYZ[\]^_
00000060 606162636465666768696a6b6c6d6e6f `abcdefghijklmno
00000070 707172737475767778797a7b7c7d7e7f pqrstuvwxyz{|}~.
00000080 808182838485868788898a8b8c8d8e8f ................
00000090 909192939495969798999a9b9c9d9e9f ................
000000a0 a0a1a2a3a4a5a6a7a8a9aaabacadaeaf ................
000000b0 b0b1b2b3b4b5b6b7b8b9babbbcbdbebf ................
000000c0 c0c1c2c3c4c5c6c7c8c9cacbcccdcecf ................
000000d0 d0d1d2d3d4d5d6d7d8d9dadbdcdddedf ................
000000e0 e0e1e2e3e4e5e6e7e8e9eaebecedeeef ................
000000f0 f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff ................
00000100 54686973206973206120737472696e67 This.is.a.string
00000110 206f6620415343494920636861726163 .of.ASCII.charac
00000120 746572732e0a                     ters..
$
```

ihad is released under the MIT license and may be used at your own risk.
However, unless it provides some specific capability you need then it
is probably better to use one of the many industry standard dump utilities.
For example on OSX or linux you could try one of the following: hexdump, xxd or od.
For comparison sake these utilities with the same input file produce; -

```
$ hexdump -C tmp.dat
00000000  00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f  |................|
00000010  10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f  |................|
00000020  20 21 22 23 24 25 26 27  28 29 2a 2b 2c 2d 2e 2f  | !"#$%&'()*+,-./|
00000030  30 31 32 33 34 35 36 37  38 39 3a 3b 3c 3d 3e 3f  |0123456789:;<=>?|
00000040  40 41 42 43 44 45 46 47  48 49 4a 4b 4c 4d 4e 4f  |@ABCDEFGHIJKLMNO|
00000050  50 51 52 53 54 55 56 57  58 59 5a 5b 5c 5d 5e 5f  |PQRSTUVWXYZ[\]^_|
00000060  60 61 62 63 64 65 66 67  68 69 6a 6b 6c 6d 6e 6f  |`abcdefghijklmno|
00000070  70 71 72 73 74 75 76 77  78 79 7a 7b 7c 7d 7e 7f  |pqrstuvwxyz{|}~.|
00000080  80 81 82 83 84 85 86 87  88 89 8a 8b 8c 8d 8e 8f  |................|
00000090  90 91 92 93 94 95 96 97  98 99 9a 9b 9c 9d 9e 9f  |................|
000000a0  a0 a1 a2 a3 a4 a5 a6 a7  a8 a9 aa ab ac ad ae af  |................|
000000b0  b0 b1 b2 b3 b4 b5 b6 b7  b8 b9 ba bb bc bd be bf  |................|
000000c0  c0 c1 c2 c3 c4 c5 c6 c7  c8 c9 ca cb cc cd ce cf  |................|
000000d0  d0 d1 d2 d3 d4 d5 d6 d7  d8 d9 da db dc dd de df  |................|
000000e0  e0 e1 e2 e3 e4 e5 e6 e7  e8 e9 ea eb ec ed ee ef  |................|
000000f0  f0 f1 f2 f3 f4 f5 f6 f7  f8 f9 fa fb fc fd fe ff  |................|
00000100  54 68 69 73 20 69 73 20  61 20 73 74 72 69 6e 67  |This is a string|
00000110  20 6f 66 20 41 53 43 49  49 20 63 68 61 72 61 63  | of ASCII charac|
00000120  74 65 72 73 2e 0a                                 |ters..|
00000126
$
```

```
$ od -A x -t x1z tmp.dat
000000 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f  >................<
000010 10 11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f  >................<
000020 20 21 22 23 24 25 26 27 28 29 2a 2b 2c 2d 2e 2f  > !"#$%&'()*+,-./<
000030 30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f  >0123456789:;<=>?<
000040 40 41 42 43 44 45 46 47 48 49 4a 4b 4c 4d 4e 4f  >@ABCDEFGHIJKLMNO<
000050 50 51 52 53 54 55 56 57 58 59 5a 5b 5c 5d 5e 5f  >PQRSTUVWXYZ[\]^_<
000060 60 61 62 63 64 65 66 67 68 69 6a 6b 6c 6d 6e 6f  >`abcdefghijklmno<
000070 70 71 72 73 74 75 76 77 78 79 7a 7b 7c 7d 7e 7f  >pqrstuvwxyz{|}~.<
000080 80 81 82 83 84 85 86 87 88 89 8a 8b 8c 8d 8e 8f  >................<
000090 90 91 92 93 94 95 96 97 98 99 9a 9b 9c 9d 9e 9f  >................<
0000a0 a0 a1 a2 a3 a4 a5 a6 a7 a8 a9 aa ab ac ad ae af  >................<
0000b0 b0 b1 b2 b3 b4 b5 b6 b7 b8 b9 ba bb bc bd be bf  >................<
0000c0 c0 c1 c2 c3 c4 c5 c6 c7 c8 c9 ca cb cc cd ce cf  >................<
0000d0 d0 d1 d2 d3 d4 d5 d6 d7 d8 d9 da db dc dd de df  >................<
0000e0 e0 e1 e2 e3 e4 e5 e6 e7 e8 e9 ea eb ec ed ee ef  >................<
0000f0 f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd fe ff  >................<
000100 54 68 69 73 20 69 73 20 61 20 73 74 72 69 6e 67  >This is a string<
000110 20 6f 66 20 41 53 43 49 49 20 63 68 61 72 61 63  > of ASCII charac<
000120 74 65 72 73 2e 0a                                >ters..<
000126
$
```

```
$ xxd -g 0 tmp.dat
00000000: 000102030405060708090a0b0c0d0e0f  ................
00000010: 101112131415161718191a1b1c1d1e1f  ................
00000020: 202122232425262728292a2b2c2d2e2f   !"#$%&'()*+,-./
00000030: 303132333435363738393a3b3c3d3e3f  0123456789:;<=>?
00000040: 404142434445464748494a4b4c4d4e4f  @ABCDEFGHIJKLMNO
00000050: 505152535455565758595a5b5c5d5e5f  PQRSTUVWXYZ[\]^_
00000060: 606162636465666768696a6b6c6d6e6f  `abcdefghijklmno
00000070: 707172737475767778797a7b7c7d7e7f  pqrstuvwxyz{|}~.
00000080: 808182838485868788898a8b8c8d8e8f  ................
00000090: 909192939495969798999a9b9c9d9e9f  ................
000000a0: a0a1a2a3a4a5a6a7a8a9aaabacadaeaf  ................
000000b0: b0b1b2b3b4b5b6b7b8b9babbbcbdbebf  ................
000000c0: c0c1c2c3c4c5c6c7c8c9cacbcccdcecf  ................
000000d0: d0d1d2d3d4d5d6d7d8d9dadbdcdddedf  ................
000000e0: e0e1e2e3e4e5e6e7e8e9eaebecedeeef  ................
000000f0: f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff  ................
00000100: 54686973206973206120737472696e67  This is a string
00000110: 206f6620415343494920636861726163   of ASCII charac
00000120: 746572730a                        ters.
$
```

OR if you have Microsoft Windows powershell and are ok with unicode text output,
instead of strictly ASCII output; -

```
> format-hex C:\test\tmp.dat


           Path: C:\test\tmp.dat

           00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F

00000000   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F  ................
00000010   10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F  ................
00000020   20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F   !"#$%&'()*+,-./
00000030   30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F  0123456789:;<=>?
00000040   40 41 42 43 44 45 46 47 48 49 4A 4B 4C 4D 4E 4F  @ABCDEFGHIJKLMNO
00000050   50 51 52 53 54 55 56 57 58 59 5A 5B 5C 5D 5E 5F  PQRSTUVWXYZ[\]^_
00000060   60 61 62 63 64 65 66 67 68 69 6A 6B 6C 6D 6E 6F  `abcdefghijklmno
00000070   70 71 72 73 74 75 76 77 78 79 7A 7B 7C 7D 7E 7F  pqrstuvwxyz{|}~
00000080   80 81 82 83 84 85 86 87 88 89 8A 8B 8C 8D 8E 8F  
00000090   90 91 92 93 94 95 96 97 98 99 9A 9B 9C 9D 9E 9F  
000000A0   A0 A1 A2 A3 A4 A5 A6 A7 A8 A9 AA AB AC AD AE AF   ¡¢£¤¥¦§¨©ª«¬­®¯
000000B0   B0 B1 B2 B3 B4 B5 B6 B7 B8 B9 BA BB BC BD BE BF  °±²³´µ¶·¸¹º»¼½¾¿
000000C0   C0 C1 C2 C3 C4 C5 C6 C7 C8 C9 CA CB CC CD CE CF  ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏ
000000D0   D0 D1 D2 D3 D4 D5 D6 D7 D8 D9 DA DB DC DD DE DF  ÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞß
000000E0   E0 E1 E2 E3 E4 E5 E6 E7 E8 E9 EA EB EC ED EE EF  àáâãäåæçèéêëìíîï
000000F0   F0 F1 F2 F3 F4 F5 F6 F7 F8 F9 FA FB FC FD FE FF  ðñòóôõö÷øùúûüýþ.
00000100   54 68 69 73 20 69 73 20 61 20 73 74 72 69 6E 67  This is a string
00000110   20 6F 66 20 41 53 43 49 49 20 63 68 61 72 61 63   of ASCII charac
00000120   74 65 72 73 2E 0A                                ters..          
>
```

With so many well established utilites to dump a file or stdin, why bother to create another?
Well... the output of ihad is somewhat cleaner than the output from the utility programs mentioned above.
The default output of ihad has just a single space between each of the three columns of output.
(The one exception to this philosophy is that there may be multiple padding spaces in the last line
of the dump if it is short of the full compliment of bytes.)
In contrast "hexdump -C" puts in extra space characters and book-ends the
Ascii column with the "|" character, "od -A x -t x1z" puts in extra space characters
and puts ">" "<" bookends around the Ascii column and "xxd -g 0" puts a colon on the end of the index number.

The ihad command has a number of options which are outlined in the useage information. Start the utility
with the "-h" (help) option to see all the other options that are available; -

```
> ihad.exe -h

Useage:
ihad [options] [inputFile1 [inputFile2 [.. inputFileN]]]
  where options are '-A -b X -B X -c C -d -D -f X -h -H -I -L X -o outfileName -s -S C -v[X] -w X'; -
   -A .. Disable output of ASCII column (bytes in ASCII form normally in 3rd column)
   -b X .. Begin file dumps at an offset (X > 0 offset from start, X < 0 offset from end)
   -B X .. Limit file dumps to a maximum of X bytes
   -c C .. Use char C instead of '.' as substitute for a non-printable or non-ASCII char (also overides -A)
   -C .. Cryptogram mode output enable - i.e. shortcut for -I -H -w32 -v6
   -d .. Decimal index output enable & default to 10 bytes per line
   -D .. Debug output enable
   -f X .. Set hex field separator to X spaces (where 0 <= X <= 2)
   -h .. Print out this help message and exit
   -H .. Disable output of Hex column (bytes in Hexadecimal form normally in 2nd column)
   -I .. Disable output of Index column (Index number of first byte in a line, normally in 1st column)
   -L X .. Limit file dumps to a maximum of X lines
   -o outfileName .. Specify an output file instead of sending output to stdout
   -s .. Classify space char as printable in Ascii output
   -S C .. Use char C instead of default space (SP) char as column separator char
   -v[X] .. Verbose output enable, optionally set level to X (where 0 <= X <= 6)
   -w X .. Set bytes per line to X (where 0 < X <= 32)

  where; -
   [inputFile1 [inputFile2 [.. inputFileN]]]  are optional file name(s)
    of file(s) to dump in hex & ascii
   Note that if no input file is specified then input is taken from stdin.

Note that if ihad output isn't acceptable you can try; -
xxd -g 0
hexdump -C
od -A x -t x1z -v
```

If the displaying the period character (i.e. full stop) at end of ASCII sentences is
important then the -c C option enables the choice of a different indicator of non-ASCII
or non-printable ASCII bytes. For example underscore could be specified with "-c_"
and produces the following style dump; -
```
$ ./ihad -c_ demo.bin
00000000 000102030405060708090a0b0c0d0e0f ________________
00000010 101112131415161718191a1b1c1d1e1f ________________
00000020 202122232425262728292a2b2c2d2e2f _!"#$%&'()*+,-./
```

If a different column separator is desirable, instead of the space character, then
it may be specified with the -S C option. For example a pseudo comma separated variable (CSV)
output may be simulated with "-S,". This option dump will have the following style; -
```
$ ./ihad -S, demo.bin
00000000,000102030405060708090a0b0c0d0e0f,................
00000010,101112131415161718191a1b1c1d1e1f,................
00000020,202122232425262728292a2b2c2d2e2f,.!"#$%&'()*+,-./
```
Or if it desired to have every hexadecimal byte separated by a comma; -
```
$ ./ihad -S , -f 1 -B 44 demo.bin 
00000000,,00,01,02,03,04,05,06,07,08,09,0a,0b,0c,0d,0e,0f,,................
00000010,,10,11,12,13,14,15,16,17,18,19,1a,1b,1c,1d,1e,1f,,................
00000020,,20,21,22,23,24,25,26,27,28,29,2a,2b,,,,,,,,,,,,,,.!"#$%&'()*+
```
Note however, that the last line in this pseudo CSV case can have too many commas,
if it is shorter than the normal line length.

The character to use may be specified in similar ways to the standard UNIX/Linux
translate utility "tr" if the character if desired. For example a tab delimited
dump could be done as follows; -
```
$ ./ihad -S '\t' demo.bin 
00000000	000102030405060708090a0b0c0d0e0f	................
00000010	101112131415161718191a1b1c1d1e1f	................
00000020	202122232425262728292a2b2c2d2e2f	.!"#$%&'()*+,-./
```

The dump can begin at an offset into a file, using the -b X option, where the offset X is
a decimal number smaller than the size of the file. If the X value is greater than zero then
the offset is applied from the start of the file. If the X value is negetive then the offset
is applied back from the end of the file.
For example; -
```
$ ./ihad -b 16 demo.bin
00000010 101112131415161718191a1b1c1d1e1f ................
00000020 202122232425262728292a2b2c2d2e2f .!"#$%&'()*+,-./
$ ./ihad -b -16 demo.bin
00000020 202122232425262728292a2b2c2d2e2f .!"#$%&'()*+,-./
```

The dump can be limited to a maximum number of lines or a maximum number of bytes.
The "-L X" option limits the file dump to a maximum of X lines. In a similar
fashion the option "-B X" limits the file dump to a maximum of X bytes. If both
"-L" and "-B" options are used at the same time then the one resulting in the
smallest number of bytes being dumped is chosen. Examples of -L or -B use are; -
```
$ ./ihad -L 1 demo.bin 
00000000 000102030405060708090a0b0c0d0e0f ................
$ ./ihad -B 16 demo.bin 
00000000 000102030405060708090a0b0c0d0e0f ................
```

Summary information about the characters in files can be obtained by using "-v3" and
above vebosity levels. For example  "ihad -I -H -A -v3 demo.bin" could be used to
show just summary information about file "demo.bin"; -
```
$ ./ihad -I -H -A -v3 demo.bin
File: 'demo.bin' is 48 bytes
Summary: ASCII: 48 chars in total of which 16 are printable (includes spaces)
Summary: ASCII: 0 alphabet ( 0 upper case and 0 lower case ) chars
Summary: ASCII: 0 digit, 15 punctuation, 1 space and 32 control chars
Summary: ASCII: 1 space (SP) and 1 horizontal tab (HT) chars
Summary: ASCII: 1 carriage return (CR), 1 line feed (LF) and 1 full stop chars
File: 'demo.bin' (48 bytes processed)
```

Option "-v5" provides more details including a listing of frequency of all bytes.
Option "-v4" provides a listing of only bytes that have non-zero frequencies.

A cryptogram mode has been added with the "-C" option. The "-C" option is just a
shortcut to specifying "-I -H -w32 -v6". For example; -
```
$ ./ihad -C test/ASD_Coin_Level2.encoded 
File: 'test/ASD_Coin_Level2.encoded' is 79 bytes
.MWNVGRXFOLFHRMVCVXFGRLM..URMWXO
ZIRGBRM7DRWGSC5WVKGS..DVZIVZFWZX
RLFHRMXLMXVKGZ.
Summary: ASCII: 75 chars in total of which 74 are printable (includes spaces)
Summary: ASCII: 71 alphabet ( 71 upper case and 0 lower case ) chars
Summary: ASCII: 2 digit, 1 punctuation, 0 space and 1 control chars
Char:  A  B  C  D  E  F  G  H  I  J  K  L  M  N  O  P  Q  R  S  T  U  V  W  X  Y  Z 
%Frq:  0  1  3  3  0  7  8  3  3  0  3  6 10  1  3  0  0 13  3  0  1 10  7  8  0  7 

%Frq: 13 10 10  8  8  7  7  7  6  3  3  3  3  3  3  3  1  1  1  0  0  0  0  0  0  0 
Char:  R  M  V  G  X  F  W  Z  L  C  D  H  I  K  O  S  B  N  U  A  E  J  P  Q  T  Y 
File: 'test/ASD_Coin_Level2.encoded' (79 bytes processed)
```
Note that the "-v6" option maybe used on its own without reference to "-C".
For example; -
```
$ ./ihad -v6 -w24 test/ASD_Coin_Level2.encoded 
File: 'test/ASD_Coin_Level2.encoded' is 79 bytes
00000000 2e4d574e56475258464f4c4648524d564356584647524c4d .MWNVGRXFOLFHRMVCVXFGRLM
00000018 c2b755524d57584f5a49524742524d374452574753433557 ..URMWXOZIRGBRM7DRWGSC5W
00000030 564b4753c2b744565a49565a46575a58524c4648524d584c VKGS..DVZIVZFWZXRLFHRMXL
00000048 4d58564b475a0a                                   MXVKGZ.
Summary: ASCII: 75 chars in total of which 74 are printable (includes spaces)
Summary: ASCII: 71 alphabet ( 71 upper case and 0 lower case ) chars
Summary: ASCII: 2 digit, 1 punctuation, 0 space and 1 control chars
Char:  A  B  C  D  E  F  G  H  I  J  K  L  M  N  O  P  Q  R  S  T  U  V  W  X  Y  Z 
%Frq:  0  1  3  3  0  7  8  3  3  0  3  6 10  1  3  0  0 13  3  0  1 10  7  8  0  7 

%Frq: 13 10 10  8  8  7  7  7  6  3  3  3  3  3  3  3  1  1  1  0  0  0  0  0  0  0 
Char:  R  M  V  G  X  F  W  Z  L  C  D  H  I  K  O  S  B  N  U  A  E  J  P  Q  T  Y 
File: 'test/ASD_Coin_Level2.encoded' (79 bytes processed)
```
