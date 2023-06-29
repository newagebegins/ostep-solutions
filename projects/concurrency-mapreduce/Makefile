wordcount: wordcount.c mapreduce.o
	clang -g -Wall -Werror -o wordcount wordcount.c mapreduce.o

mapreduce.o: mapreduce.c mapreduce.h
	clang -g -c -Wall -Werror -o mapreduce.o mapreduce.c

clean:
	rm -f wordcount mapreduce.o
