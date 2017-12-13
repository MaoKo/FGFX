$SKIP {
    SPACE       =  / [ \t\n]+                              / ;
    COMMENT     =  / (\/\/.*)|(\/\*(\*+[^*\/]|[^*])*\*+\/) / ;
};

$TOKEN {
    @LETTER     =  / [a-zA-Z_]                             / ;
    @DIGIT      =  / [0-9]                                 / ;
    @IDENT      =  / {LETTER}({LETTER}|{DIGIT})*           / ;

    /* FGFL */
    LOCAL_TOK   =  / @{IDENT}                              / ;
    EQUAL       =  / =                                     / ;
    REGEX       =  / \/([^\/\\\n]|\\(.|\n))*\/             / ;
    
    /* FGFP */
    NTER        =  / <{IDENT}'?>                           / ;
    ARROW       =  / ->                                    / ;
    UNION       =  / \|                                    / ;
    EMPTY       =  / %empty%                               / ;
    LITERAL     =  / '([^\n\\'"]|\\[\\'"])+'               / ;
    
    /* Both FGFL & FGFP */
    DIRECTIVE   =  / ${IDENT}                              / ;
    GLOBAL_TOK  =  / {IDENT}                               / ;
    BARROW      =  / =>                                    / ;
    SEMI        =  / ;                                     / ;
    COMMA       =  / ,                                     / ;
    LBRACE      =  / \{                                    / ;
    RBRACE      =  / \}                                    / ;
    LPAREN      =  / \(                                    / ;
    RPAREN      =  / \)                                    / ;
};

$KEYWORD {
    SKIP, TOKEN, KEYWORD, IGCASE,
    EXTERN, PRODUCTION, ALIAS, START,
};

