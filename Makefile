all:
		bison -d interpreter.y
		flex -o interpreter.yy.c interpreter.l
		gcc -g interpreter.tab.c interpreter.yy.c stack.c main.c
clean:
		rm -f a.out *.h interpreter.tab.c interpreter.yy.c
