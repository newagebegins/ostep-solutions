all: wordcount wordcount_array

wordcount: wordcount.c mapreduce.o
	clang -g -Wall -Werror -o wordcount wordcount.c mapreduce.o

mapreduce.o: mapreduce.c mapreduce.h
	clang -g -c -Wall -Werror -o mapreduce.o mapreduce.c

wordcount_array: wordcount.c mapreduce_array.o
	clang -g -Wall -Werror -o wordcount_array wordcount.c mapreduce_array.o

mapreduce_array.o: mapreduce_array.c mapreduce.h
	clang -g -c -Wall -Werror -o mapreduce_array.o mapreduce_array.c

clean:
	rm -f wordcount wordcount_array mapreduce.o mapreduce_array.o
