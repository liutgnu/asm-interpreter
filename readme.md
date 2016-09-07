# Overview #
---
## Introduction ##

The interpreter can interpret and run assembly-like language script. It supports 8 instructions:

	1)  mov
	2)  cmp
	3)  je
	4)  jmp
	5)  print
	6)  call
	7)  ret
	8)  halt

And 52 registers:

	a-z A-Z

The interpreter contains an arithmetic calculator, so there is no need to bring in arithmetic instructions.


**For example:**

	mov x,1                          means move 1 to x
	mov x,x+y                        means move the sum of x+y to x
	mov x,(x+y)*(x-4)
	print "hello,world"              prints string hello,world
	call func1                       call a new function func1
	ret                              return from a function

## To use: ##


- program the assembly-like language in a file, test.s for example.
- run the program by type the following commmand in terminal

	./interpreter test.s

- voilà, that's it!

## The purpose of assembly-like language interpreter ##

- tool to learn assembly language programming for newbies
- have fun programming anywhere by cross-compile it to Android phones

## NOTE: ##
This interpreter doesn't support floating-point arithmetics such as "mov x,3.14", or code comment.
Be ware!
