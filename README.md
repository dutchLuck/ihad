# ihad
ihad is named as such because it provides "index" "hex" "ascii" "dump" of a file.
"ihad" is a command line program that by default dumps file data in a format of multiple lines of three columns
separated by a space character. The first column is an index number representing an offset from the start of the
file to the leftmost data byte in the second and third columns, the second column is hexadecimal representation
of the bytes of data in the file and the third column is an ASCII representation of the bytes of data in the file.

$ ./ihad tmp.bin

00000000 54686973206973206120737472696e67 This.is.a.string

00000010 206f6620636861726163746572730a   .of.characters.

$

ihad is not production code! Consider it only slightly tested.
PLEASE do not use it for anything serious! Instead use; -


hexdump -C yourFile

xxd yourFile

od -A x -t x1z -v yourFile

OR

http://www.fileformat.info/tool/hexdump.htm 

