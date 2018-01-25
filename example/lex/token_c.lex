//////////////////////////////
// C Token Definition (C11) //
//////////////////////////////

$TOKEN
{
    IDENTIFIER  =   / ({LETTER}|{ESC_UNV})({LETTER}|{DIGIT}|{ESC_UNV})*/ ;
    INT_CONST   =   / ({DEC}|{OCT}|{HEX}){INT_SUFF}? / ;
    FLOAT_CONST =   / ({DEC_FLOAT}|{HEX_FLOAT}) / ;
    CHAR_CONST  =   / [LuU]?'([^\n'\\]|{REG_ESCAPE})+' / ;
    STRING      =   / (u8?|[UL])?\"([^\n"\\]|{REG_ESCAPE})*\" / ;

    /* C Identifier fragment */

    LETTER      =   / [[:alpha:]_] / -> { $FRAG };
    DIGIT       =   / [[:digit:]]  / -> { $FRAG };

    /* C Char/String fragment */

    ESC_UNV     =   / (\\u[[:xdigit:]]{4}|\\U[[:xdigit:]]{8}) / -> { $FRAG };
    ESC_OCT     =   / (\\[0-7]{1,3})     / -> { $FRAG };
    ESC_HEX     =   / (\\x[[:xdigit:]]+) / -> { $FRAG };
    ESC_SEQ     =   / \\['"?\\abfnrtv]   / -> { $FRAG } ;

    REG_ESCAPE  =   / ({ESC_SEQ}|{ESC_OCT}|{ESC_HEX}|{ESC_UNV}) / -> { $FRAG } ;

    /* C Number Constant Fragment */

    /* C Integer Fragment */

    DEC         =   / ([1-9][[:digit:]]*)  / -> { $FRAG };
    OCT         =   / (0[0-7]*)            / -> { $FRAG };
    HEX         =   / (0[xX][[:xdigit:]]+) / -> { $FRAG };

    UNSIGN_SUFF =   / [uU]    / -> { $FRAG };
    LONG_SUFF   =   / [lL]    / -> { $FRAG };
    LGLG_SUFF   =   / (ll|LL) / -> { $FRAG };

    INT_SUFF    =   /   ({UNSIGN_SUFF}{LONG_SUFF}?)|\
                        ({UNSIGN_SUFF}{LGLG_SUFF})|\
                        ({LONG_SUFF}{UNSIGN_SUFF}?)|\
                        ({LGLG_SUFF}{UNSIGN_SUFF}?) / -> { $FRAG };

    /* C Float Fragment */

    SIGN        =   / [-+]   / -> { $FRAG };
    FLOAT_SUFF  =   / [flFL] / -> { $FRAG };

    SCF_EXP     =   / [eE]{SIGN}?{DIGIT}+ / -> { $FRAG };
    BIN_EXP     =   / [pP]{SIGN}?{DIGIT}+ / -> { $FRAG };

    DEC_FRACT   =   / ({DIGIT}+\.)|({DIGIT}+?\.{DIGIT}+) / -> { $FRAG };
    HEX_FRACT   =   /   ([[:xdigit:]]+\.)|\
                        ([[:xdigit:]]+?\.[[:xdigit:]]+) / -> { $FRAG };

    DEC_FLOAT   =   /   ({DEC_FRACT}{SCF_EXP}?{FLOAT_SUFF}?)|\
                        ({DIGIT}+{SCF_EXP}{FLOAT_SUFF}?) / -> { $FRAG };

    HEX_FLOAT   =   / 0[xX]({HEX_FRACT}|[[:xdigit:]]){BIN_EXP}{FLOAT_SUFF} / ;

    /* C Punctuation */

    LBRACK      =   / \[    / ;
    RBRACK      =   /  ]    / ;
    LPAREN      =   / \(    / ;
    RPAREN      =   / \)    / ; 
    LBRACE      =   / \{    / ;
    RBRACE      =   /  }    / ;
    DOT         =   / \.    / ;
    ARROW       =   /  ->   / ;

    PLUSPLUS    =   / "++"  / ;
    MINUSMINUS  =   /  --   / ;
    ANDB        =   /  &    / ;
    STAR        =   / \*    / ;
    PLUS        =   / \+    / ;
    MINUS       =   /  -    / ;
    TILDE       =   /  ~    / ;
    EXCLA       =   / \!    / ;

    DIV         =   / \/    / ;
    MOD         =   /  %    / ;
    LSHIFT      =   /  <<   / ;
    RSHIFT      =   /  >>   / ;
    LESS        =   /  <    / ;
    GREAT       =   /  >    / ;
    LESSE       =   /  <=   / ;
    GREATE      =   /  >=   / ;
    EQ          =   /  ==   / ;
    NEQ         =   /  !=   / ;
    XOR         =   / \^    / ;
    ORB         =   / \|    / ;
    ORL         =   / "||"  / ;
    ANDL        =   /  &&   / ;

    QUES        =   / \?    / ;
    COLON       =   /  :    / ;
    SEMICOLON   =   / \;    / ;
    ELLIP       =   / "..." / ;

    ASSIGN      =   /  =    / ;
    MULE        =   / \*=   / ;
    DIVE        =   / \/=   / ;
    MODE        =   /  %=   / ;
    PLUSE       =   / \+=   / ;
    MINUSE      =   /  -=   / ;
    LSHIFTE     =   /  <<=  / ;
    RSHIFTE     =   /  >>=  / ;
    ANDBE       =   /  &=   / ;
    XORE        =   / \^=   / ;
    ORBE        =   / \|=   / ;

    COMMA       =   /  ,    / ;
    DASH        =   /  #    / ;
    DASHDASH    =   /  ##   / ;

    DLBRACK     =   /  <:   / ;
    DRBRACK     =   /  :>   / ;
    DLBRACE     =   /  <%   / ;
    DRBRACE     =   /  %>   / ;
    DDASH       =   /  %:   / ;
    DDASHDASH   =   /  %:%: / ;
};

$SKIP
{
    /* C Comment */

    COMMENT     =   / (\/\/.*)|(\/\*(\*+[^*\/]|[^*])*\*+\/) / ;

    /* C Space */

    SPACE       =   / [ \t\n]+ / ;
};

$KEYWORD
{
    AUTO,       IF,         UNSIGNED,
    BREAK,      INLINE,     VOID,
    CASE,       INT,        VOLATILE,
    CHAR,       LONG,       WHILE,
    CONST,      REGISTER,   _ALIGNAS,
    CONTINUE,   RESTRICT,   _ALIGNOF,
    DEFAULT,    RETURN,     _ATOMIC,
    DO,         SHORT,      _BOOL,
    DOUBLE,     SIGNED,     _COMPLEX,
    ELSE,       SIZEOF,     _GENERIC,
    ENUM,       STATIC,     _IMAGINARY,
    EXTERN,     STRUCT,     _NORETURN,
    FLOAT,      SWITCH,     _STATIC_ASSERT,
    FOR,        TYPEDEF,    _THREAD_LOCAL,
    GOTO,       UNION,
};
