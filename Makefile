build:
	gcc -g *.c -o main

test: build
	valgrind --leak-check=full ./main

clean:
	rm main output*.out
