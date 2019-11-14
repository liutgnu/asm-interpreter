        mov    y, 1
        jmp     .L2:
.L5:
        mov    x, 0
        jmp     .L3:
.L4:
        print "*"
        mov    x, x+1
.L3:
	cmp x, y
        jl      .L4:
	print "\n"
	mov y, y+1
.L2:
        cmp    y, 10
        jle     .L5:
	halt
