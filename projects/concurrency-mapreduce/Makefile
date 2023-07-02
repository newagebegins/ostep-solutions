my_flags = -O -Wall -Werror

all: wordcount wordcount_array wordcount_array_values

wordcount: wordcount.c mapreduce.o
	clang $(my_flags) -o wordcount wordcount.c mapreduce.o

mapreduce.o: mapreduce.c mapreduce.h
	clang -c $(my_flags) -o mapreduce.o mapreduce.c

wordcount_array: wordcount.c mapreduce_array.o
	clang $(my_flags) -o wordcount_array wordcount.c mapreduce_array.o

mapreduce_array.o: mapreduce_array.c mapreduce.h
	clang -c $(my_flags) -o mapreduce_array.o mapreduce_array.c

wordcount_array_values: wordcount.c mapreduce_array_values.o
	clang $(my_flags) -o wordcount_array_values wordcount.c mapreduce_array_values.o

mapreduce_array_values.o: mapreduce_array_values.c mapreduce.h
	clang -c $(my_flags) -o mapreduce_array_values.o mapreduce_array_values.c

clean:
	rm -f wordcount wordcount_array mapreduce.o mapreduce_array.o mapreduce_array_values.o
