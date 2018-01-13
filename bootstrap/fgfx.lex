$STATE
{
	GLOBAL => $INITIAL,
	SEEN_REGEX,
};

$TOKEN
{
    LETTER = / [a-zA-Z_] / -> { $FRAG } ;
    DIGIT  = / [0-9]     / -> { $FRAG } ;

    /* FGFL */

    EQUAL = /  = / ;
	STAR  = / \* / ;
   
    ( SEEN_REGEX ) REGEX = / ([^\/\\\n]|\\(.|\n))* / ;

    /* FGFP */

    NON_TERMINAL =  / <{TERMINAL}('{1,3})?>   / ;
    LITERAL      =  / '([^\n\\'"]|\\[\\'"])+' / ;

    UNION  = / \| / ;

    LBRACK = / \[ / ;
    RBRACK = / \] / ;
    
    /* Both FGFL & FGFP */

    DIRECTIVE = / ${TERMINAL}                 / ;
    TERMINAL  = / {LETTER}({LETTER}|{DIGIT})* / ;

	ARROW  = / -> / ;
    BARROW = / => / ;

    SEMI   =  / ; / ;
    COMMA  =  / , / ;

    LBRACE = / \{ / ;
    RBRACE = / \} / ;

    LPAREN = / \( / ;
    RPAREN = / \) / ;
};

$SKIP
{
	( GLOBAL ) BEG_REGEX     = / \/[ \t]* /, ( $BEGIN SEEN_REGEX ) ;
	( SEEN_REGEX ) END_REGEX = / [ \t]*\/ /, ( $BEGIN GLOBAL     ) ;

    SPACE   = / [ \t\n]+                              / ;
    COMMENT = / (\/\/.*)|(\/\*(\*+[^*\/]|[^*])*\*+\/) / ;
};

$KEYWORD
{
	// FGFL
    SKIP, TOKEN, KEYWORD, IGCASE, STATE,
	BEGIN, FRAG, INITIAL, ALL, NONE,

	/* TODO */
	REJECT,

	// FGFP
    EXTERN, PRODUCTION, ALIAS, PRECEDENCE, MIMIC,
	EMPTY, START, LEFT, RIGHT, NONASSOC,
};

