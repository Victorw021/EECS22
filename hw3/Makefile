all: PhotoLab PhotoLabTest

PhotoLab.o: PhotoLab.c Constants.h DIPs.h Advanced.h FileIO.h
	gcc -c -Wall -ansi -std=c11 PhotoLab.c -o PhotoLab.o

PhotoLabTest.o: PhotoLab.c Constants.h DIPs.h Advanced.h FileIO.h
	gcc -c -DDEBUG -Wall -ansi -std=c11 PhotoLab.c -o PhotoLabTest.o

Advanced.o: Advanced.c Advanced.h Constants.h
	gcc -c -Wall -ansi -std=c11 Advanced.c -o Advanced.o

DIPs.o: DIPs.c DIPs.h Constants.h
	gcc -c -Wall -ansi -std=c11 DIPs.c -o DIPs.o

FileIO.o: Constants.h FileIO.c FileIO.h
	gcc -c -Wall -ansi -std=c11 FileIO.c -o FileIO.o

libfilter.a: DIPs.o Advanced.o
	ar rc libfilter.a DIPs.o Advanced.o
	ranlib libfilter.a

PhotoLab: PhotoLab.o FileIO.o libfilter.a
	gcc -lm -Wall -ansi -std=c11 PhotoLab.o FileIO.o -L. -lfilter -o PhotoLab

PhotoLabTest: PhotoLabTest.o FileIO.o libfilter.a
	gcc -lm -Wall -ansi -std=c11 PhotoLabTest.o FileIO.o -L. -lfilter -o PhotoLabTest

clean:
	rm -f *.o
	rm -f PhotoLab
	rm -f PhotoLabTest
