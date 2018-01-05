$SKIP
{
    SPACE       =  / [ \t\n]+                              / ;
    COMMENT     =  / (\/\/.*)|(\/\*(\*+[^*\/]|[^*])*\*+\/) / ;
};

$TOKEN
{
    @LETTER     =  / [a-zA-Z_]                             / ;
    @DIGIT      =  / [0-9]                                 / ;
    @IDENT      =  / {LETTER}({LETTER}|{DIGIT})*           / ;

    /* FGFL */
    LOCAL_TOK   =  / @{IDENT}                              / ;
    REGEX       =  / \/([^\/\\\n]|\\(.|\n))*\/             / ;
	STAR		=  / \*                                    / ;    

    /* FGFP */
    NTER        =  / <{IDENT}('{1,3})?>                    / ;
    BARROW      =  / =>                                    / ;
    UNION       =  / \|                                    / ;
    LBRACK      =  / \[                                    / ;
    RBRACK      =  / \]                                    / ;
    LITERAL     =  / '([^\n\\'"]|\\[\\'"])+'               / ;
    
    /* Both FGFL & FGFP */
    DIRECTIVE   =  / ${IDENT}                              / ;
    GLOBAL_TOK  =  / {IDENT}                               / ;
    ARROW       =  / ->                                    / ;
    EQUAL       =  / =                                     / ;
    SEMI        =  / ;                                     / ;
    COMMA       =  / ,                                     / ;
    LBRACE      =  / \{                                    / ;
    RBRACE      =  / \}                                    / ;
    LPAREN      =  / \(                                    / ;
    RPAREN      =  / \)                                    / ;
};

$KEYWORD
{
	// FGFL
    SKIP, TOKEN, KEYWORD, IGCASE,
	STATE, BEGIN, FRAG, INITIAL,

	// FGFP
    EXTERN, PRODUCTION, ALIAS, PRECEDENCE, MIMIC,
	EMPTY, START, LEFT, RIGHT, NONASSOC,
};

