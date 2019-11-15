        mov    ebx, 1
        jmp     .L2:
.L5:
        mov    eax, 0
        jmp     .L3:
.L4:
        print "*"
        mov    eax, eax+1
.L3:
	cmp eax, ebx
        jl      .L4:
	print "\n"
	mov ebx, ebx+1
.L2:
        cmp    ebx, 10
        jle     .L5:
	halt
