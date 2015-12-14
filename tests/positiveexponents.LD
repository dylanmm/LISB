# Positive exponents
[DEF expnt [number exponenet] 
	[SET ret 1]
	[LOOP exponenet
		[SET ret [* ret number]]]
	[IF [== ret 0] [SET ret 1]]
	[ret]]


[SET counter 1]
[LOOP 10 
	[PRINT [expnt 2 counter]]
	[SET counter [+ counter 1]]]