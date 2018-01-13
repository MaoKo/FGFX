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

	( BEG_CCL, BODY_CCL ) CCE   = / "[:{LETTER}:]"  /, ( $BEGIN BODY_CCL );
	( BEG_CCL, BODY_CCL ) N_CCE = / "[:^{LETTER}:]" /, ( $BEGIN BODY_CCL );

	( STRING, BEG_CCL, BODY_CCL ) CHAR = / .|\\(.|\n) /,
			( $BEGIN $NONE, BODY_CCL, $NONE ) ;
};

$KEYWORD
{
	// Character Class Expression
	CCE_ALNUM,  /* [:alnum:]  */
	CCE_ALPHA,  /* [:alpha:]  */
	CCE_CNTRL,  /* [:cntrl:]  */
	CCE_DIGIT,  /* [:digit:]  */
	CCE_GRAPH,  /* [:graph:]  */
	CCE_LOWER,  /* [:lower:]  */
	CCE_PRINT,  /* [:print:]  */
	CCE_PUNCT,  /* [:punct:]  */
	CCE_SPACE,  /* [:space:]  */
	CCE_UPPER,  /* [:upper:]  */
	CCE_XDIGIT, /* [:xdigit:] */

	// Negate Character Class Expression
	N_CCE_ALNUM,  /* [:^alnum:]  */
	N_CCE_ALPHA,  /* [:^alpha:]  */
	N_CCE_CNTRL,  /* [:^cntrl:]  */
	N_CCE_DIGIT,  /* [:^digit:]  */
	N_CCE_GRAPH,  /* [:^graph:]  */
	N_CCE_LOWER,  /* [:^lower:]  */
	N_CCE_PRINT,  /* [:^print:]  */
	N_CCE_PUNCT,  /* [:^punct:]  */
	N_CCE_SPACE,  /* [:^space:]  */
	N_CCE_UPPER,  /* [:^upper:]  */
	N_CCE_XDIGIT, /* [:^xdigit:] */
};

