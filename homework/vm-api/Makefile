all: null nofree 5 6 7 vec_test

null: null.c
	gcc -Wall -g -o null null.c

nofree: nofree.c
	gcc -Wall -g -o nofree nofree.c

5: 5.c
	gcc -Wall -g -o 5 5.c

6: 6.c
	gcc -Wall -g -o 6 6.c

7: 7.c
	gcc -Wall -g -o 7 7.c

vec_test: vec_test.c vec.o
	gcc -Wall -g -o vec_test vec_test.c vec.o

vec.o: vec.c vec.h
	gcc -Wall -g -c vec.c

clean:
	rm -f null nofree 5 6 7 vec_test vec.o
