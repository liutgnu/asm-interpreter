# Overview #

## Update 2 ##

Recently I have done some projects which involved disassembly and reverse engineering. I realised maybe the interpreter can work as an emulator to help debug a whole or only a small section of the target binary. Not all binary files can be loaded by debugger, and directly reading assembly program is hard. It will be better having an emulator which can tell by emulating a few instructions, what memory address and cpu resigters were changed, what functions were called etc. 

Project is still in development, code can be found in x86 branch.

## Update 1 ##

I have rewritten the entire project with flex and bison, which is more flexible and extensive. The original version can be found in master branch.

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
	call func1:                      call a new function func1
	ret                              return from a function

## To use: ##


- program the assembly-like language in a file, test.s for example.
- run the program by type the following commmand in terminal

	$ ./a.out test.s

- voilà, that's it!
- *.s files are samples of assembly-like program.

## The purpose of assembly-like language interpreter ##

- tool to learn assembly language programming for newbies
- have fun programming anywhere by cross-compile it to Android phones

## The output of sample program ##

$ ./a.out loop_branch_test.s

```
*
**
***
****
*****
******
*******
********
*********
```

$ ./a.out function_call_test.s
```
hello
in func1
in func2
in func3
world
```
$ ./a.out calculation_test.s
```
395.640000
```

