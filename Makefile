calc:
		bison -d calc.y
		flex -o calc.yy.c calc.l
		gcc -g calc.tab.c calc.yy.c main.c
clean:
		rm -f a.out *.h calc.tab.c calc.yy.c
