$STATE
{
	GLOBAL => $INITIAL,
	STRING,
	FINITE_SEQ,
	BEG_CCL,
	BODY_CCL,
};

$TOKEN
{
	( GLOBAL ) UNION = / \| / ;
	( GLOBAL ) STAR = / \* / ;
	( GLOBAL ) PLUS = / \+ / ;
	( GLOBAL ) QUES = / \? / ;
	( GLOBAL ) LPAREN = / \( / ;
	( GLOBAL ) RPAREN = / \) / ;
	( GLOBAL ) DOT = / \. / ;

	( GLOBAL, STRING ) QUOTE = / \" /, ( $BEGIN STRING, GLOBAL ) ;

	LETTER = / [a-zA-Z_]+ / -> { $FRAG } ;
	( GLOBAL ) BOUND_NAME = / \{{LETTER}({LETTER}|{DIGIT})*\} /;
	
	( GLOBAL ) LBRACE = / \{ /, ( $BEGIN FINITE_SEQ ) ;
	( FINITE_SEQ ) DIGIT = / [0-9]+ / ;
	( FINITE_SEQ ) COMMA = / , / ;
	( FINITE_SEQ ) RBRACE = / \} /, ( $BEGIN GLOBAL ) ;

	( GLOBAL ) DIFF_CLASS = / "{-}" / ;
	( GLOBAL ) UNION_CLASS = / "{+}" / ;

	( GLOBAL ) LBRACK = / \[ /, ( $BEGIN BEG_CCL ) ;
	( BEG_CCL ) CARET = / ^ /, ( $BEGIN BODY_CCL ) ;

	( BODY_CCL ) HYPHEN = / - / ;
	( BODY_CCL ) RBRACK = / \] /, ( $BEGIN GLOBAL ) ;

	( STRING, BEG_CCL, BODY_CCL ) CHAR = / .|\\(.|\n) /,
			( $BEGIN $NONE, BODY_CCL, $NONE ) ;
};
