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
    EQUAL       =  / =                                     / ;
    REGEX       =  / \/([^\/\\\n]|\\(.|\n))*\/             / ;
    
    /* FGFP */
    NTER        =  / <{IDENT}('{1,3})?>                    / ;
    ARROW       =  / ->                                    / ;
    UNION       =  / \|                                    / ;
    LITERAL     =  / '([^\n\\'"]|\\[\\'"])+'               / ;
    
    /* Both FGFL & FGFP */
    DIRECTIVE   =  / ${IDENT}                              / ;
    GLOBAL_TOK  =  / {IDENT}                               / ;
    BARROW      =  / =>                                    / ;
    SEMI        =  / ;                                     / ;
    COMMA       =  / ,                                     / ;
    LBRACE      =  / \{                                    / ;
    RBRACE      =  / \}                                    / ;
    LBRACK      =  / \[                                    / ;
    RBRACK      =  / \]                                    / ;
    LPAREN      =  / \(                                    / ;
    RPAREN      =  / \)                                    / ;
};

$KEYWORD
{
    SKIP, TOKEN, KEYWORD, IGCASE,
    EXTERN, PRODUCTION, ALIAS, START,
	PRECEDENCE, EMPTY, LEFT, RIGHT,
	NONASSOC, MIMIC,
};

