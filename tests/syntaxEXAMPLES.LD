# SYNTAX EXAMPLE

[0]
[PRINT [0]]

[0 1 2 3 4]
[PRINT [0 1 2 3 4]]

[[1] [2] [3]]
[[1][2][3]]
[[
	[0]	]]
[+ 1 [3]]

[PRINT [+ 1 [+ 1 [3]]]]
[PRINT [+ 1 [3]]]
[PRINT [3]]
[PRINT [PRINT 7]]

[DEF addone n [+ n 1]]

[DEF addtwo [n] 
	[SET x 0]
	[SET x [+ n 2]]
	[x]]

[PRINT 
	[addone 3]]
[PRINT 
	[addtwo 2]]

[SET j 2]
[SET b 6]
[SET j b]

[LOOP 2 [PRINT 1337]]
[LOOP [+ 1 1] [PRINT 1337]]


# Print 1 - 10
[SET counter 1]
[LOOP 10 
	[PRINT counter]
	[SET counter [+ counter 1]]]

[DEF addthree [x y z] [+ x [+ y z]]]
[PRINT 
[addthree 1 2 3]]


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






