FGFL (Fast Generate, Fast Lexer) and FGFP (Fast Generate, Fast Parser)
are scientific tools.
The purpose of FGFL is to create from lexical specification (example/lex/ folder)
a DFA ready for lexical analysis. Note that the DFA is suboptimal, in the future,
I attempt to fix this. 
The goal of FGFP is to create from a grammar description a LALR(1) parsing table.
Also for speed, maybe, I replace the Vector with Hash.
I also no free memory or only a subset. I try to fix this as soon as possible.

### TODO ###
	.	More accurate handle of error in FGFX
	.	Correct the bitset when the value is too big
	.	Add the %start directive in FGFP
	.	Handle of FIRST and FOLLOW set
