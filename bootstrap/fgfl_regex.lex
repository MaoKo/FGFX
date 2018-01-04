$STATE
{
	INITIAL,
	BEG_CLASS, BODY_CLASS
	CURLY,
};

$TOKEN
{
	@LETTER = / [A-Za-z_]+ / ;

	( INITIAL ) LBRACK = / \[ / ;
	( INITIAL ) LBRACE = / \{ / ;

	( CURLY ) DIGIT  = / [0-9]+ / ;
	( CURLY ) IDENT  = / {LETTER}({LETTER}|{DIGIT})*  /;
	( CURLY ) RBRACE = / } / ;

	( BEG_CLASS, INITIAL ) CARET = / ^ / ;
	( BEG_CLASS, BODY_CLASS ) RBRACK = / ] / ;

	// last rule for catch all other char
	( * ) CHAR = / (.|\n) / ;
};
