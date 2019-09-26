# ihad
ihad is named as such because it provides "index" "hex" "ascii" "dump" of a file.
"ihad" is a command line program that by default dumps file data in a format of multiple lines of three columns
separated by a space character. The first column is an index number representing an offset from the start of the
file to the leftmost data byte in the second and third columns, the second column is hexadecimal representation
of the bytes of data in the file and the third column is an ASCII representation of the bytes of data in the file.
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
Well... the output of ihad is somewhat cleaner (i.e. just a space between columns in the default output) than the output from the utility programs mentioned above.

The ihad command has a number of options which are outlined in the useage information.

```
$ ./ihad -h

Useage:
./ihad [options] [inputFile1 [inputFile2 [.. inputFileN]]]
  where options are '-A -d -D -f X -h -I -o outfileName -s -v X -w X'; -
   -A .. Ascii output disable
   -d .. Decimal index output enable & default to 10 bytes per line
   -D .. Debug output enable
   -f X .. Set hex field separator to X spaces (where 0 < X < 2)
   -h .. Print out this help message and exit
   -I .. Index output disable
   -o outfileName .. Specify an output file instead of sending output to stdout
   -s .. classify space char as printable in Ascii output
   -v X .. Set verbosity level to X (where 0 < X < 4)
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

