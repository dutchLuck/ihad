ihad: ihad.o
	gcc -o $@ $<

ihad.o: ihad.c
	gcc -Wall -c $<

check:
	cd test; ./chk

clean:
	rm -f *.o *.exe ihad

