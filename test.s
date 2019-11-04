mov x,1
mov y,1
mov z,5
print x
print y
loop:
mov x,x+y
print x
mov y,x+y
print y
mov z,z-1
cmp z,0
je end:
jmp loop:
end:
