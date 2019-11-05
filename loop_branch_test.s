mov z, 10
mov y, 1
outerLoop:
mov x, 0
cmp y, z
je outerEnd:
innerLoop:
cmp x, y
je innerFinish:
print "*"
mov x, x+1
jmp innerLoop:
innerFinish:
print "\n"
mov y, y+1
jmp outerLoop:
outerEnd:
halt
