interpreter: src/interpreter
	(cp src/interpreter ./)

src/interpreter:
	(cd src;make)

clean:
	rm -f interpreter
	(cd src;make clean)
