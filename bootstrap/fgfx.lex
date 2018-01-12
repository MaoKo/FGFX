$SKIP
{
    SPACE        =  / [ \t\n]+                              / ;
    COMMENT      =  / (\/\/.*)|(\/\*(\*+[^*\/]|[^*])*\*+\/) / ;
};

$TOKEN
{
    LETTER       =  / [a-zA-Z_]                             / -> { $FRAG } ;
    DIGIT        =  / [0-9]                                 / -> { $FRAG } ;

    /* FGFL */
    EQUAL        =  / =                                     / ;
    REGEX        =  / \/([^\/\\\n]|\\(.|\n))*\/             / ;
	STAR		 =  / \*                                    / ;    

    /* FGFP */
    NON_TERMINAL =  / <{TERMINAL}('{1,3})?>                 / ;

    UNION        =  / \|                                    / ;

    LBRACK       =  / \[                                    / ;
    RBRACK       =  / \]                                    / ;

    LITERAL      =  / '([^\n\\'"]|\\[\\'"])+'               / ;
    
    /* Both FGFL & FGFP */
    DIRECTIVE    =  / ${TERMINAL}                           / ;
    TERMINAL     =  / {LETTER}({LETTER}|{DIGIT})*           / ;

    ARROW        =  / ->                                    / ;
    BARROW       =  / =>                                    / ;

    SEMI         =  / ;                                     / ;
    COMMA        =  / ,                                     / ;

    LBRACE       =  / \{                                    / ;
    RBRACE       =  / \}                                    / ;

    LPAREN       =  / \(                                    / ;
    RPAREN       =  / \)                                    / ;
};

$KEYWORD
{
	// FGFL
    SKIP, TOKEN, KEYWORD, IGCASE, STATE,
	BEGIN, FRAG, INITIAL, ALL, NONE,

	// FGFP
    EXTERN, PRODUCTION, ALIAS, PRECEDENCE, MIMIC,
	EMPTY, START, LEFT, RIGHT, NONASSOC,
};

