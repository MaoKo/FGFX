FGFL (Fast Generate, Fast Lexer) and FGFP (Fast Generate, Fast Parser)
are scientific tools.
The purpose of FGFL is to create from lexical specification (example/lex/ folder)
a DFA ready for lexical analysis. Note that the DFA is suboptimal, in the future,
I attempt to fix this. 
The goal of FGFP is to create from a grammar description a LALR(1) parsing table.
Also for speed, maybe, I replace the Vector with Hash.
I also no free memory or only a subset. I try to fix this as soon as possible.
I want to, before anything, create the LR(1) automata and after merging similar state with same LR(0) items.
It's seem dumb because I can use the technique of propagate and spontaneous lookahead drawn from the Dragon Book.
But I prefer do that, if the user want to conserve the LR(1) table.

### How to make? ###
Git clone the repository.
Then, type: make

After you can do: ./efgfl example/lex/token_c.lex
Which create a file called token_c.h.lex ready for lexing.

### TODO ###
	.	More accurate handle of error in FGFX (FGFP done)
	.	Optimizing FOLLOW set
	.	Handle if file already exist
	.	Improve the algorithm for constructing the LALR parsing table
	.	Add error recovery mechanism
	.	Add parser and lexer in the template dir
	.	Add in FGFP ( ... | ... )
	.	Add $STATE in FGFL
	.	Add $FRAG and $BEGIN in FGFL
	.	Add $BEGIN in FGFL
	.	Create the grammar and the lexical specification for regex
	.	Learn MarkDown :)

