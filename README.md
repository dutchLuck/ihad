# ihad
ihad is named as such because it provides "index" "hex" "ascii" "dump" of a file.
"ihad" is a command line program that by default dumps file data in a format of multiple lines of three columns
separated by a space character. The first column is an index number representing an offset from the start of the
file to the leftmost data byte in the second and third columns, the second column is hexadecimal representation
of the bytes of data in the file and the third column is shows any ASCII characters in the bytes of data in the file.
An example of the default output of ihad is; -

```
$ ./ihad tmp.bin

00000000 54686973206973206120737472696e67 This.is.a.string

00000010 206f6620636861726163746572730a   .of.characters.

$
```

ihad is not production code! Consider it only slightly tested.
PLEASE do not use it for anything serious! Instead use; -

```
hexdump -C yourFile     (linux)
```
```
xxd yourFile
```
```
od -A x -t x1z -v yourFile
```
OR

```
http://www.fileformat.info/tool/hexdump.htm 
```
OR

```
format-hex yourFile   (Windows Power Shell)
```

With so many well established utilites to dump a file, why bother to create another?
Well... the output of ihad is somewhat cleaner (i.e. just a single space between each of the three columns in the default output, except multiple padding spaces in the last line if it is short on bytes) than the output from the utility programs mentioned above. For example "xxd -g 0" puts a colon on the end of the index number, "hexdump -C" puts in lots of space characters and book-ends the
Ascii column with the "|" character and "od -A x -t x1z -v" puts in lots of space characters and ">" "<" bookends around the Ascii column.

The ihad command has a number of options which are outlined in the useage information.

```
> ihad.exe -h

Useage:
ihad.exe [options] [inputFile1 [inputFile2 [.. inputFileN]]]
  where options are '-A -c C -d -D -f X -h -H -I -o outfileName -s -v X -w X'; -
   -A .. Ascii output disable
   -c C .. Set char C as non-ASCII indicator
   -d .. Decimal index output enable & default to 10 bytes per line
   -D .. Debug output enable
   -f X .. Set hex field separator to X spaces (where 0 < X < 2)
   -h .. Print out this help message and exit
   -H .. Hexadecimal output disable
   -I .. Index output disable
   -o outfileName .. Specify an output file instead of sending output to stdout
   -s .. Classify space char as printable in Ascii output
   -v X .. Verbose output enable, set level to X (where 0 < X < 4)
   -w X .. Set bytes per line to X (where 0 < X <= 32)

  where; -
   [inputFile1 [inputFile2 [.. inputFileN]]]  are optional file name(s)
    of file(s) to dump in hex & ascii
   Note that if no input file is specified then input is taken from stdin.

Note that if ihad output isn't acceptable you can try; -
xxd
hexdump -C
od -A x -t x1z -v
```
If the full stop at end of a ASCII sentences is important then the -c C option enables the choice of a different
indicator of non-ASCII bytes, for example underscore could be specified with -c _
