FGFL (Fast Generate, Fast Lexer) and FGFP (Fast Generate, Fast Parser)
are scientific tools.
The purpose of FGFL is to create from lexical specification (example/lex/ folder)
a DFA ready for lexical analysis. Note that the DFA is suboptimal, in the future,
I attempt to fix this. 
The goal of FGFP is to create from a grammar description a LALR(1) parsing table.
Also for speed, maybe, I replace the Vector with Hash.
I also no free memory or only a subset. I try to fix this as soon as possible.

### How to make? ###
Git clone the repository.
Then, type: make

After you can do: ./efgfl example/lex/token_c.lex
Which create a file called token_c.h.lex ready for lexing.

### TODO ###
	.	More accurate handle of error in FGFX
	.	Optimizing FOLLOW set
	.	Handle if file already exist
	.	Allow forward reference in FGFL
	.	No output in FGFL when the file is empty
	.	Allow optional elements in FGFP with [ ... ]
	.	Allow list elements in FGFP with { ... }
	.	Allow several alias for the same terminal
	.	Improve the algorithm for constructing the LALR parsing table
	.	Learn MarkDown :)

