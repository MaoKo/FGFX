/////////////////////////////////////////////////
// Lexical Specification of both (FGFL & FGFP) //
/////////////////////////////////////////////////

@LETTER		=	[a-zA-Z_] ;
@DIGIT		=	[0-9] ;
@IDENT		=	{LETTER}({LETTER}|{DIGIT})* ;

/* FGFL */
LOCAL_TOK	=	@{IDENT} ;
EQUAL		=	= ;
IGCASE		=	%igcase ;
SKIP		=	%skip ;
//TODO: beg regex with dollar
//DOLLAR	=	$ ;

/* FGFP */
NTER		=	<{IDENT}'?> ;
ARROW		=	-> ;
UNION		=	\| ;
STR		=	\"[^\n\"]+\" ;

/* Both */
GLOBAL_TOK	=	{IDENT} ;
START		=	%start ;
TOKEN		=	%token ;

SEMI		=	\; ;
COMMA		=	, ;

SPACE		=	[ \t\n]+ ;
COM		=	(//.*)|(/\*(\*+[^*/]|[^*])*\*+/) ;

%skip		SPACE, COM ;

