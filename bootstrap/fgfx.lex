/////////////////////////////////////////////////
// Lexical Specification of both (FGFL & FGFP) //
/////////////////////////////////////////////////

@LETTER		=	[a-zA-Z_] ;
@DIGIT		=	[0-9] ;
@IDENT		=	{LETTER}({LETTER}|{DIGIT})* ;

/* FGFL */
LOCAL_TOK	=	@{IDENT} ;
EQUAL		=	= ;
//TODO: beg regex with dollar
//DOLLAR	=	$ ;

/* FGFP */
NTER		=	<{IDENT}'?> ;
ARROW		=	-> ;
UNION		=	\| ;
@ESCAPE		=	\\["'] ;
LITERAL		=	'([^\n']|{ESCAPE})' ;
STR		=	\"([^\n"]|{ESCAPE})+\" ;

/* Both FGFL & FGFP */
DIRECTIVE	=	%{IDENT} ;
GLOBAL_TOK	=	{IDENT} ;
SEMI		=	\; ;
COMMA		=	, ;
SPACE		=	[ \t\n]+ ;
COM		=	(//.*)|(/\*(\*+[^*/]|[^*])*\*+/) ;

%keyword		START, TOKEN, IGCASE, SKIP, KEYWORD, ALIAS ;
%skip			SPACE, COM ;

