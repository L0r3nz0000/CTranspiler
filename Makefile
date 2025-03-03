all:
	gcc -g -c src/main.c -o bin/main.o
	gcc -g -c src/parser.c -o bin/parser.o
	gcc -g -c src/lexer.c -o bin/lexer.o
	gcc -g -c src/code_generator.c -o bin/code_generator.o
	gcc bin/main.o bin/parser.o bin/lexer.o bin/code_generator.o -o main -lpcre2-8
	./main src.l

test:
	gcc -g -c test.c -o bin/test.o
	gcc -g -c src/parser.c -o bin/parser.o
	gcc -g -c src/lexer.c -o bin/lexer.o
	gcc -g -c src/code_generator.c -o bin/code_generator.o
	gcc bin/test.o bin/parser.o bin/lexer.o bin/code_generator.o -o test -lpcre2-8
	./test