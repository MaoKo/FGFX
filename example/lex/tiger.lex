$STATE
{
    GLOBAL => $INITIAL,
    COMMENT,
    IN_STRING,
};

$TOKEN
{
    /* Identifier */

    IDENT  = / [a-zA-Z][a-zA-Z0-9_]* / ;

    /* Integer Literal */

    INT    = / [[:digit:]]+ / ;

    /* String */
    (IN_STRING) STRING = / \" /, ( $BEGIN (GLOBAL) ) ;

    /* Punctuation symbol */

    COMMA  = /  , / ;
    COLON  = /  : / ;
    SEMI   = /  ; / ;
    LPAREN = / \) / ;
    RPAREN = / \( / ;
    LBRACK = / \[ / ;
    RBRACK = /  ] / ;
    LBRACE = / \{ / ;
    RBRACE = /  } / ;
    DOT    = / \. / ;
    PLUS   = / \+ / ;
    MINUS  = /  - / ; 
    TIME   = / \* / ;
    DIV    = / \/ / ;
    EQUAL  = /  = / ;
    DIFF   = / <> / ;
    LESS   = /  < / ;
    GREAT  = /  > / ;
    LESSE  = / <= / ;
    GREATE = / >= / ;
    AND    = /  & / ;
    OR     = / \| / ;
    ASSIGN = / := / ; 
};

$SKIP
{
    /* String */

    BEG_STRING = / \" /, ( $BEGIN (IN_STRING) ) ;

    (IN_STRING) NL_STR  = / \\n / ;
    (IN_STRING) TAB_STR = / \\t / ;

    (IN_STRING) CTRL_STR = / \\^[0abvfre] / ;

    (IN_STRING) DEC_STR = / \\[[:digit:]]{1,3} / ;

    (IN_STRING) ESC_QUOTE = / \\\" / ;
    (IN_STRING) ESC_SLASH = / \\\\ / ;
 
    (IN_STRING) IGNORE_STR  = / \\[ \t\n\f]+\\ / ;
    (IN_STRING) CHAR_STR    = / [^\n\\] / ;

    /* Comment */

    OPEN_COM = / "/*" /, ( $BEGIN (COMMENT) ) ;

    (COMMENT) CHAR_COM  = / (?s:.) / ;
    (COMMENT) CLOSE_COM = / \*+\/  /, ( $BEGIN (GLOBAL) ) ;

    /* Space */

    SPACE = / [ \n\t]+ / ;
};

$KEYWORD
{
    WHILE, FOR, TO,
    BREAK, LET, IN,
    END, FUNCTION, VAR,
    TYPE, ARRAY, IF,
    THEN, ELSE, DO,
    OF, NIL,

    // TYPE_ID is an IDENT defined by a type declaration
    TYPE_ID,
};
