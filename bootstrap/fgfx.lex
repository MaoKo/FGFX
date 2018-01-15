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
    ( GLOBAL ) BEG_REGEX   = / \/[[:blank:]]* /,  ( $BEGIN IN_REGEX ) ;
   
    /* Regex */

    ( IN_REGEX ) END_REGEX = / [[:blank:]]*\//{SPACE}*(,|;|->) /,
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

    ( BEG_CCL, BODY_CCL ) CCE   = / "[:"{LETTER}+":]" /,  ( $BEGIN BODY_CCL ) ;
    ( BEG_CCL, BODY_CCL ) N_CCE = / "[:^"{LETTER}+":]" /, ( $BEGIN BODY_CCL ) ;

    ( IN_REGEX, STRING, BEG_CCL, BODY_CCL, ) OCT_NUM = / \\[0-7]{1,3} /,
                            ( $BEGIN $NONE, $NONE, BODY_CCL, $NONE ) ;

    ( IN_REGEX, STRING, BEG_CCL, BODY_CCL, ) HEX_NUM = / \\x[a-f0-9]{1,2} /
                    -> { $IGCASE }, ( $BEGIN $NONE, $NONE, BODY_CCL, $NONE ) ;

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
    CC_FIRST, // Dummy

	// Character Class Expression
    CCE_ALNUM, CCE_ALPHA,
    CCE_CNTRL, CCE_DIGIT,
    CCE_GRAPH, CCE_LOWER,
    CCE_PRINT, CCE_PUNCT,
    CCE_SPACE, CCE_BLANK,
    CCE_UPPER, CCE_XDIGIT,

    CC_MIDDLE, // Dummy

	// Negate Character Class Expression
    N_CCE_ALNUM, N_CCE_ALPHA,
    N_CCE_CNTRL, N_CCE_DIGIT,
    N_CCE_GRAPH, N_CCE_LOWER,
    N_CCE_PRINT, N_CCE_PUNCT,
    N_CCE_SPACE, N_CCE_BLANK,
    N_CCE_UPPER, N_CCE_XDIGIT,

    CC_LAST, // Dummy

	// FGFP
    EXTERN, PRODUCTION, ALIAS, PRECEDENCE, MIMIC,
    EMPTY, START, LEFT, RIGHT, NONASSOC,
};

