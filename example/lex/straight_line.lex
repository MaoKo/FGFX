$SKIP
{
	SPACE = / [ \t\n]+ / ;
};

$TOKEN
{
	ID = / [[:alpha:]_]([[:alpha:]_]|[[:digit:]])* / ;
	NUM = / [[:digit:]]+ / ;
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
