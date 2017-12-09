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
@ESCAPE		=	\\["'] ;
LITERAL		=	'([^\n']|{ESCAPE})' ;
STR		=	\"([^\n"]|{ESCAPE})+\" ;
START		=	%start ;
TOKEN		=	%token ;

/* Both */
GLOBAL_TOK	=	{IDENT} ;
SEMI		=	\; ;
COMMA		=	, ;
SPACE		=	[ \t\n]+ ;
COM		=	(//.*)|(/\*(\*+[^*/]|[^*])*\*+/) ;

%skip		SPACE, COM ;

