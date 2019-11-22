main:
        push    rbp
        mov     rbp, rsp
        mov     rsp, rsp-16
        mov     DWORD PTR -16[rbp], 2
        mov     DWORD PTR -12[rbp], 3
	mov	rdx,rbp -12
	mov 	rax,rbp -16
        mov     rsi, rdx
        mov     rdi, rax
        call    swap
        mov     edx, DWORD PTR -12[rbp]
        mov     eax, DWORD PTR -16[rbp]
	print   eax
	print	"\n"
	print 	edx
	mov 	rsp, rbp
	pop	rbp
	halt
swap:
        push    rbp
        mov     rbp, rsp
        mov     QWORD PTR -24[rbp], rdi
        mov     QWORD PTR -32[rbp], rsi
        mov     rax, QWORD PTR -24[rbp]
        mov     eax, DWORD PTR [rax]
        mov     DWORD PTR -4[rbp], eax
        mov     rax, QWORD PTR -32[rbp]
        mov     edx, DWORD PTR [rax]
        mov     rax, QWORD PTR -24[rbp]
        mov     DWORD PTR [rax], edx
        mov     rax, QWORD PTR -32[rbp]
        mov     edx, DWORD PTR -4[rbp]
        mov     DWORD PTR [rax], edx
        pop     rbp
        ret

