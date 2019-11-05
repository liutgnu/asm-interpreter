print "hello\n"
call func1:
print "world\n"
halt

func1:
print "in func1\n"
call func2:
ret

func2:
print "in func2\n"
call func3:
ret

func3:
print "in func3\n"
ret
