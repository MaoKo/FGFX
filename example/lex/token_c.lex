//////////////////////////////
// C Token Definition (C11) //
//////////////////////////////

/* C Identifier token */

$TOKEN
{
    ESC_UN      =   /(\\u[[:xdigit:]]{4}|\\U[[:xdigit:]]{8})/ -> { $FRAG };
    LETTER      =   /[[:alpha:]_] / -> { $FRAG };
    DIGIT       =   /[[:digit:]]  / -> { $FRAG };
    IDENT       =   /({LETTER}|{ESC_UN})({LETTER}|{DIGIT}|{ESC_UN})*/ ;

    /* C Char token */

    ESC_OCT     =   /(\\[0-7]{1,3})     / -> { $FRAG };
    ESC_HEX     =   /(\\x[[:xdigit:]]+) / -> { $FRAG };
    ESC_SEQ     =   /\\['"?\\abfnrtv]   / -> { $FRAG } ;

    REG_ESCAPE  =   /({ESC_SEQ}|{ESC_OCT}|{ESC_HEX}|{ESC_UN})/ -> { $FRAG } ;
    CHAR        =   /[LuU]?'([^\n'\\]|{REG_ESCAPE})+'/ ;

    /* C String token */

    STR         =   /(u8?|[UL])?\"([^\n"\\]|{REG_ESCAPE})*\"/ ;

    /* C Punctuation */

    COMMA       =   /  ,    / ;
    SEMICOLON   =   / \;    / ;
    LPAREN      =   / \(    / ;
    RPAREN      =   / \)    / ; 
    LBRACK      =   / \{    / ;
    RBRACK      =   /  }    / ;
    LSUB        =   / \[    / ;
    RSUB        =   /  ]    / ;
    QUES        =   / \?    / ;
    ELLIP       =   / "..." / ;
    EXCLA       =   /  !    / ;

    PLUS        =   / \+    / ;
    PLUSPLUS    =   / "++"  / ;
    MINUS       =   /  -    / ;
    MINUSMINUS  =   /  --   / ;
    DIV         =   / \/    / ;
    MUL         =   / \*    / ;
    MOD         =   /  %    / ;
    XOR         =   / \^    / ;
    ORB         =   / \|    / ;
    ANDB        =   /  &    / ;
    ORL         =   / "||"  / ;
    ANDL        =   /  &&   / ;
    TILDE       =   /  ~    / ;
    ASSIGN      =   /  =    / ;

    PLUSE       =   / \+=   / ;
    MINUSE      =   /  -=   / ;
    DIVE        =   / \/=   / ;
    MULE        =   / \*=   / ;
    MODE        =   /  %=   / ;
    XORE        =   / \^=   / ;
    ORBE        =   / \|=   / ;
    ANDBE       =   /  &=   / ;
    ORLE        =   / "||=" / ;
    ANDLE       =   /  &&=  / ;
    TILDEE      =   /  ~=   / ;

    EQ          =   /  ==   / ;
    NEQ         =   /  !=   / ;
    LESS        =   /  <    / ;
    GREAT       =   /  >    / ;
    LESSE       =   /  <=   / ;
    GREATE      =   /  >=   / ;

    COLON       =   /  :    / ;
    DASH        =   /  #    / ;

    /* C Number */

    /* C Int */

    DEC         =   /([1-9][0-9]*)/ -> { $FRAG };
    OCT         =   /(0[0-7]*)/ -> { $FRAG };
    HEX         =   /(0[xX][0-9a-fA-F]+)/ -> { $FRAG };
    U_S         =   /[uU]/ -> { $FRAG };
    L_S         =   /[lL]/ -> { $FRAG };
    LL_S        =   /(ll|LL)/ -> { $FRAG };

    INT_S       =   /(({U_S}{L_S}?)|({U_S}{LL_S})|\
                                    ({L_S}{U_S}?)|({LL_S}{U_S}?))/ -> { $FRAG };

    INTEGER     =   /({DEC}|{OCT}|{HEX}){INT_S}?/ ;

    /* C Float */
    SIGN        =   /[-+]/   -> { $FRAG };
    F_S         =   /[flFL]/ -> { $FRAG };

    SCF_EXP     =   /[eE]{SIGN}?{DIGIT}+/ -> { $FRAG };
    BIN_EXP     =   /[pP]{SIGN}?{DIGIT}+/ -> { $FRAG };

    D_FRACT     =   /({DIGIT}+\.)/ -> { $FRAG };

    DECF        =   /({D_FRACT}{SCF_EXP}?{F_S}?)|\
                                    ({DIGIT}+{SCF_EXP}{F_S}?)/ -> { $FRAG };
    FLOAT       =   /{D_FRACT}/ ;
};

$SKIP
{
    /* C Comment */
    COMMENT     =   /(\/\/.*)|(\/\*(\*+[^*\/]|[^*])*\*+\/)/ ;

    /* C Space */
    SPACE       =   /[ \t\n]+/ ;
};

