all:
		bison -d interpreter.y
		flex -o interpreter.yy.c interpreter.l
		gcc -g interpreter.tab.c interpreter.yy.c stack.c cpu.c main.c
clean:
		rm -f a.out interpreter.tab.h interpreter.tab.c interpreter.yy.c
