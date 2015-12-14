#____ _   _ _  _ ___ ____ _  _    ____ _  _ ____ _  _ ___  _    ____ ____ 
#[__   \_/  |\ |  |  |__|  \/     |___  \/  |__| |\/| |__] |    |___ [__  
#___]   |   | \|  |  |  | _/\_    |___ _/\_ |  | |  | |    |___ |___ ___] 
#                                                                         
# Examples of valid syntax

[0]
# Returns 0

[PRINT [0]]
# Outputs the return value, which is zero

[0 1 2 3 4]
# Returns 4
[PRINT [0 1 2 3 4]]
# Outputs 4


[[1] [[2]][3]]
[[[1][2][[3]]]]
# ALL VALID
[[
	[0]	]]
# White space is ignored

[+ 1 [3]]
# Returns 4

[PRINT [+ 1 [+ 1 [3]]]]
# Outputs 5
[PRINT [+ 1 [3]]]
# Outputs 4
[PRINT [3]]
# Outputs 3

[PRINT [PRINT 7]]
# Outputs 7, then 1 because arguments are evaluted first, and a successful print 
# returns a value of 1. So the return value of PRINT 7 is 1.

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







