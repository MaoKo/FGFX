$STATE
{
    GLOBAL => $INITIAL,
    BEG_REGEX, BODY_REGEX,
    BEG_CCL, BODY_CCL,
    STRING,
    FINITE_SEQ,
    NESTED_COM,
};

$TOKEN
{
    /* Fragment */

    LETTER = / [[:alpha:]_] / -> { $FRAGMENT } ;

    /* FGFL */

    EQUAL = /  = / ;
    STAR  = / \* / ;
    ( GLOBAL ) OPEN_REGEX = / \/[[:blank:]]* /, ( $BEGIN (BEG_REGEX) ) ;
   
    /* Regex */

    ( BEG_REGEX, BODY_REGEX ) 
        CLOSE_REGEX = / [[:blank:]]*\//{SPACE}*(,|;|->) /,
        ( $BEGIN (GLOBAL, *) ) ;

    ( BEG_REGEX, BODY_REGEX ) REG_UNION  = / \| /, ( $BEGIN (BODY_REGEX) ) ;
    ( BEG_REGEX, BODY_REGEX ) REG_STAR   = / \* /, ( $BEGIN (BODY_REGEX) ) ;
    ( BEG_REGEX, BODY_REGEX ) REG_PLUS   = / \+ /, ( $BEGIN (BODY_REGEX) ) ;
    ( BEG_REGEX, BODY_REGEX ) REG_QUES   = / \? /, ( $BEGIN (BODY_REGEX) ) ;
    ( BEG_REGEX, BODY_REGEX ) REG_LPAREN = / \( /, ( $BEGIN (BODY_REGEX) ) ;
    ( BEG_REGEX, BODY_REGEX ) REG_RPAREN = / \) /, ( $BEGIN (BODY_REGEX) ) ;
    ( BEG_REGEX, BODY_REGEX ) REG_DOT    = / \. /, ( $BEGIN (BODY_REGEX) ) ;
    ( BEG_REGEX, BODY_REGEX ) REG_LOOK   = / \/ /, ( $BEGIN (BODY_REGEX) ) ;

    ( BEG_REGEX, BODY_REGEX, STRING )
        REG_QUOTE = / \" /,
        ( $BEGIN (STRING, STRING, BODY_REGEX) ) ;

    ( BEG_REGEX, BODY_REGEX )
        REG_BOUND_NAME =  / \{{LETTER}({LETTER}|{DIGIT})*\} /,
        ( $BEGIN (BODY_REGEX) ) ;
 
    ( BEG_REGEX, BODY_REGEX ) REG_NOT    = / \! / ;
   
    ( BEG_REGEX, BODY_REGEX ) REG_LBRACE = / \{ /, ( $BEGIN (FINITE_SEQ, *) ) ;
    ( FINITE_SEQ )            REG_RBRACE = /  } /, ( $BEGIN (BODY_REGEX)    ) ;

    ( FINITE_SEQ ) DIGIT = / [[:digit:]]+ / ;
    ( FINITE_SEQ ) REG_COMMA = / , / ;

    ( BODY_REGEX ) REG_DIFF_CLASS  = / "{-}" / ;
    ( BODY_REGEX ) REG_UNION_CLASS = / "{+}" / ;
    ( BODY_REGEX ) REG_INTER_CLASS = / "{&}" / ;

    ( BEG_REGEX, BODY_REGEX ) REG_LBRACK = / \[ /, ( $BEGIN (BEG_CCL, *)    ) ;
    ( BEG_CCL, BODY_CCL )     REG_RBRACK = /  ] /, ( $BEGIN (BODY_REGEX, *) ) ;

    ( BEG_REGEX, BEG_CCL ) REG_CARET = / \^ /,
        ( $BEGIN (BODY_REGEX, BODY_CCL) ) ;

    ( BODY_REGEX ) REG_DOLLAR = / $/[[:blank:]]*\/ / ;

    ( BODY_CCL ) REG_HYPHEN = / -/[^\]\n] / ;

    ( BEG_CCL, BODY_CCL ) CCE   = / "[:"{LETTER}+":]" /, 
        ( $BEGIN (BODY_CCL) ) ;

    ( BEG_CCL, BODY_CCL ) N_CCE = / "[:^"{LETTER}+":]" /,
        ( $BEGIN (BODY_CCL) ) ;

    ( BEG_CCL, BEG_REGEX, BODY_REGEX, STRING, BODY_CCL, )
            OCT_NUM = / \\[0-7]{1,3} /,
            ( $BEGIN (BODY_CCL, BODY_REGEX) ) ;

    ( BEG_CCL, BEG_REGEX, BODY_REGEX, STRING, BODY_CCL, )
            HEX_NUM = / \\[xX][[:xdigit:]]{1,2} /,
            ( $BEGIN (BODY_CCL, BODY_REGEX) ) ;

    ( BEG_CCL, BEG_REGEX, BODY_REGEX, STRING, BODY_CCL, )
            REG_CHAR = / \\?. /,
            ( $BEGIN (BODY_CCL, BODY_REGEX) ) ;

    /* FGFP */

    NON_TERMINAL =  / <{TERMINAL}('{1,3})?>   / ;
    LITERAL      =  / '([^\n\\'"]|\\[\\'"])+' / ;

    UNION  = / \| / ;

    LBRACK = / \[ / ;
    RBRACK = /  ] / ;
    
    /* Both FGFL & FGFP */

    DIRECTIVE = / ${TERMINAL}                 / ;
    TERMINAL  = / {LETTER}({LETTER}|{DIGIT})* / ;

    ARROW  = / -> / ;
    BARROW = / => / ;

    SEMI   =  / ; / ;
    COMMA  =  / , / ;

    LBRACE = / \{ / ;
    RBRACE = /  } / ;

    LPAREN = / \( / ;
    RPAREN = / \) / ;
};

$SKIP
{
    /* Space */
    SPACE       = / [ \t\n]+ / ;

    /* Comments */
    SINGLE_LINE                         = / \/\/.* / ;
    ( GLOBAL, NESTED_COM ) BEG_MULTI    = / \/\*   /,  ( $BEGIN (NESTED_COM) ) ;

    ( NESTED_COM ) CHAR_COMMENT         = / (.|\n) / ;
    ( NESTED_COM ) END_MULTI            = / \*+\/  /,  ( $BEGIN (GLOBAL) ) ;

    /* Multi line in  Regex */
    ( BEG_CCL, BEG_REGEX, BODY_REGEX, STRING, BODY_CCL, )
            MULTI_LINE = / (\\\n[[:blank:]]*)+ / ;
};

$KEYWORD
{
    // FGFL
    SKIP, TOKEN, KEYWORD, IGCASE, STATE,
    BEGIN, FRAGMENT, INITIAL, STAY, PUSH, POP,

    // POSIX character set
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

