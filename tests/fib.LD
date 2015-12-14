[DEF fib [n] 
	[SET ret 0]
	[IF [== n 0]
		[SET ret 0]]
	[IF [== n 1]
		[SET ret 1]]
	[IF [> n 1]
		[SET ret [+ [fib [- n 1]] [fib [- n 2]]]]]
	[ret]]

[SET j 0]

[LOOP 10 
	[PRINT [fib j]]
	[SET j [+ j 1]]]