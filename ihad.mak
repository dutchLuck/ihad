ihad: ihad.o
	gcc -o $@ $<

ihad.o: ihad.c
	gcc -Wall -c $<

clean:
	rm -f *.o *.exe ihad

