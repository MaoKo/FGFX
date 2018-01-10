$SKIP
{
	SPACE = / [ \t\n]+ / ;
};

$TOKEN
{
	ID = / [a-zA-Z_]([a-zA-Z_]|[0-9])* / ;
	NUM = / [0-9]+ / ;
	SEMI = / ; / ;
	ASSIGN = / := / ;
	LPAREN = / \( / ;
	RPAREN = / \) / ;
	COMMA = / , / ;
	PLUS = / \+ / ;
	MINUS = / - / ;
	MUL = / \* / ;
	DIV = / \/ / ;
};

$KEYWORD
{
	PRINT,
};
