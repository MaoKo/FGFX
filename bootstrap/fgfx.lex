$STATE
{
    GLOBAL => $INITIAL,
    BEG_REGEX, BODY_REGEX,
    BEG_CCL, BODY_CCL,
    STRING,
    FINITE_SEQ,
    REG_PARAMS, REG_NO_PARAMS,
    REG_COM,
    NESTED_COM,
};

$TOKEN
{
    /* Useful Fragment */

    LETTER = / [[:alpha:]_] / -> $FRAGMENT ;

    /* FGFL */

    EQUAL = /  = / ;
    STAR  = / \* / ;
    ( GLOBAL ) OPEN_REGEX = / \/[[:blank:]]* /, ( $BEGIN (BEG_REGEX) ) ;
   
    /* Regex */

    ( BODY_REGEX ) CLOSE_REGEX = / [[:blank:]]*\//{SPACE}*(,|;|->) /,
        ( $BEGIN (GLOBAL) ) ;

    ( BODY_REGEX ) REG_UNION  = / \| / ;
    ( BODY_REGEX ) REG_STAR   = / \* / ;
    ( BODY_REGEX ) REG_PLUS   = / \+ / ;
    ( BODY_REGEX ) REG_QUES   = / \? / ;

    ( BODY_REGEX ) REG_LPAREN = / \( /, ( $PUSH (BODY_REGEX) ) ;
    ( BODY_REGEX ) REG_RPAREN = / \) /, ( $POP ) ;

    ( BODY_REGEX ) REG_DOT    = / \. / ;
    ( BODY_REGEX ) REG_LOOK   = / \/ / ;

    ( BODY_REGEX, STRING ) REG_QUOTE = / \" /, ( $BEGIN (STRING, BODY_REGEX) ) ;

    ( BODY_REGEX ) REG_BOUND_NAME =  / \{{LETTER}({LETTER}|{DIGIT})*\} / ;
 
    ( BODY_REGEX ) REG_NOT    = / \! / ;
   
    ( BODY_REGEX ) REG_LBRACE = / \{ /, ( $BEGIN (FINITE_SEQ) ) ;
    ( FINITE_SEQ ) REG_RBRACE = /  } /, ( $BEGIN (BODY_REGEX) ) ;

    ( FINITE_SEQ ) DIGIT = / [[:digit:]]+ / ;
    ( FINITE_SEQ ) REG_COMMA = / , / ;

    ( BODY_REGEX ) REG_DIFF_CLASS  = / "{-}" / ;
    ( BODY_REGEX ) REG_UNION_CLASS = / "{+}" / ;
    ( BODY_REGEX ) REG_INTER_CLASS = / "{&}" / ;

    ( BODY_REGEX ) REG_LBRACK = / \[ /, ( $BEGIN (BEG_CCL)    ) ;
    ( BODY_CCL )   REG_RBRACK = /  ] /, ( $BEGIN (BODY_REGEX) ) ;

    ( BEG_REGEX, BEG_CCL ) REG_CARET = / \^ /,
        ( $BEGIN (BODY_REGEX, BODY_CCL) ) ;

    ( BODY_REGEX ) REG_DOLLAR = / \$/{CLOSE_REGEX} / ;

    ( BODY_CCL ) REG_HYPHEN = / -/[^\]\n] / ;

    ( BODY_CCL ) CCE   = / "[:"{LETTER}+":]"  / ;
    ( BODY_CCL ) N_CCE = / "[:^"{LETTER}+":]" / ;

    ( BODY_REGEX, STRING, BODY_CCL ) OCT_NUM  = / \\[0-7]{1,3} /;
    ( BODY_REGEX, STRING, BODY_CCL ) HEX_NUM  = / \\[xX][[:xdigit:]]{1,2} / ;
    ( BODY_REGEX, STRING, BODY_CCL ) REG_CHAR = / \\?. / ;

    ( BODY_REGEX ) REG_OPTION = / "(?" /, ( $PUSH (REG_PARAMS) ) ;
    ( REG_PARAMS ) REG_INVERT = / - /, ( $BEGIN (REG_NO_PARAMS) ) ;

    ( REG_PARAMS, REG_NO_PARAMS ) IGCASE  = / i / ;
    ( REG_PARAMS, REG_NO_PARAMS ) DOTALL  = / s / ;
    ( REG_PARAMS, REG_NO_PARAMS ) SKIPWS  = / x / ;
    ( REG_PARAMS, REG_NO_PARAMS ) REVERSE = / r / ;

    ( REG_PARAMS, REG_NO_PARAMS ) REG_COLON = / : /,
        ( $BEGIN (BODY_REGEX, *) ) ;

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

    SPACE = / [ \t\n]+ / ;

    /* Comments */

    SINGLE_LINE                         = / \/\/.* / ;

    ( GLOBAL, NESTED_COM ) BEG_MULTI    = / \/\*   /, 
        ( $PUSH (NESTED_COM, *) ) ;

    ( NESTED_COM ) END_MULTI            = / \*+\/  /, 
        ( $POP ) ;

    ( NESTED_COM ) CHAR_COMMENT         = / (?s:.) / ;

    /* Multi line in  Regex */

    ( BODY_REGEX, STRING, BODY_CCL, ) MULTI_LINE = / (\\\n[[:blank:]]*)+ / ;

    /* Regex comment */

    ( BEG_REGEX, BODY_REGEX ) REG_BEG_COM = / "(?#" /, ( $PUSH (REG_COM, *) ) ;
    ( REG_COM ) REG_END_COM  = / \) /, ( $POP ) ;
    ( REG_COM ) REG_CHAR_COM = / [^)]+ / ;

    ( BEG_REGEX, BEG_CCL ) REG_NOTHING = / ""(?# Move to BODY with epsilon ) /,
        ( $BEGIN (BODY_REGEX, BODY_CCL) ) ;
};

$KEYWORD
{
    // FGFL
    SKIP, TOKEN, KEYWORD, STATE, FRAGMENT,
    INITIAL, STAY, BEGIN, PUSH, POP,

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

