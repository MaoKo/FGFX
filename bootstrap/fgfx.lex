$STATE
{
	GLOBAL => $INITIAL,
	IN_REGEX,
	STRING,
	FINITE_SEQ,
	BEG_CCL,
	BODY_CCL,
};

$TOKEN
{
    /* Fragment */

    LETTER = / [[:alpha:]_] / -> { $FRAG } ;

    /* FGFL */

    EQUAL = /  = / ;
    STAR  = / \* / ;
    ( GLOBAL ) BEG_REGEX   = / \/[ \t]* /,  ( $BEGIN IN_REGEX ) ;
   
    /* Regex */

    ( IN_REGEX ) END_REGEX = / [ \t]*\//[ \t\n]*(,|;|->) /,
                                            ( $BEGIN GLOBAL   ) ;

    ( IN_REGEX ) REG_UNION = / \| / ;
    ( IN_REGEX ) REG_STAR = / \* / ;
    ( IN_REGEX ) REG_PLUS = / \+ / ;
    ( IN_REGEX ) REG_QUES = / \? / ;
    ( IN_REGEX ) REG_LPAREN = / \( / ;
    ( IN_REGEX ) REG_RPAREN = / \) / ;
    ( IN_REGEX ) REG_DOT = / \. / ;
    ( IN_REGEX ) REG_LOOK = / \/ / ;

    ( IN_REGEX, STRING ) REG_QUOTE = / \" /, ( $BEGIN STRING, IN_REGEX ) ;

    ( IN_REGEX ) REG_BOUND_NAME = / \{{LETTER}({LETTER}|{DIGIT})*\} /;
	
    ( IN_REGEX ) REG_LBRACE = / \{ /, ( $BEGIN FINITE_SEQ ) ;
    ( FINITE_SEQ ) DIGIT = / [[:digit:]]+ / ;
    ( FINITE_SEQ ) REG_COMMA = / , / ;
    ( FINITE_SEQ ) REG_RBRACE = / \} /, ( $BEGIN IN_REGEX ) ;

    ( IN_REGEX ) REG_DIFF_CLASS = / "{-}" / ;
    ( IN_REGEX ) REG_UNION_CLASS = / "{+}" / ;

    ( IN_REGEX ) REG_LBRACK = / \[ /, ( $BEGIN BEG_CCL ) ;
    ( BEG_CCL ) REG_CARET = / ^ /, ( $BEGIN BODY_CCL ) ;

    ( BODY_CCL ) REG_HYPHEN = / -/[^\]\n] / ;
    ( BEG_CCL, BODY_CCL ) REG_RBRACK = / \] /, ( $BEGIN IN_REGEX, $ALL ) ;

    ( BEG_CCL, BODY_CCL ) CCE   = / "[:"{LETTER}+":]" /, ( $BEGIN BODY_CCL );
    ( BEG_CCL, BODY_CCL ) N_CCE = / "[:^"{LETTER}":]" /, ( $BEGIN BODY_CCL );

    ( IN_REGEX, STRING, BEG_CCL, BODY_CCL, ) REG_CHAR = / .|\\(.|\n) /,
                            ( $BEGIN $NONE, $NONE, BODY_CCL, $NONE ) ;

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

    // Regex
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
    CCE_BLANK,  /* [:blank:]  */
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
    N_CCE_BLANK,  /* [:^blank:]  */
    N_CCE_UPPER,  /* [:^upper:]  */
    N_CCE_XDIGIT, /* [:^xdigit:] */

	// FGFP
    EXTERN, PRODUCTION, ALIAS, PRECEDENCE, MIMIC,
    EMPTY, START, LEFT, RIGHT, NONASSOC,
};

