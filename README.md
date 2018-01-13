FGFL (Fast Generate, Fast Lexer) and FGFP (Fast Generate, Fast Parser)
are scientific tools.
The purpose of FGFL is to create from lexical specification (example/lex/ folder)
a DFA ready for lexical analysis. Note that the DFA is suboptimal, in the future,
I attempt to fix this. 
The goal of FGFP is to create from a grammar description a LALR(1) parsing table.
Also for speed, maybe, I replace the Vector with Hash.
I also no free memory or only a subset. I try to fix this as soon as possible.
For constructing the LALR parsing table, I use the step-by-step approach, which
whenever I found similar LR(0) state, I merge them.
But I prefer do that, if the user want to conserve the LR(1) table.

### How to make? ###
Git clone the repository.
Then, type: make

After you can do: ./efgfl example/lex/token_c.lex
Which create a file called token_c.h.lex ready for lexing.

### TODO ###
	.	More accurate handle of error in FGFX (FGFP (95%), FGFL(80%))
	.	Optimizing FOLLOW set
	.	Handle if file already exist
	.	Improve the algorithm for constructing the
			LALR parsing table
			DFA transition table

	.	Add error recovery mechanism
	.	Add parser and lexer in the template dir
	.	Add in FGFP ( ... | ... )
	.	Remove unreachable state
	.	Handle when the number of state is 1
	.	Check useless $BEGIN
	.	Handle empty regex
	.	Gen begin table when occurs ( $BEGIN )
	.	Add lookahead operator
	.	Add regex scanner
	.	Add several begin move for the same token name (fix when (*) occurs)
	.	Create the grammar and the lexical specification for regex
	.	Learn MarkDown :)

