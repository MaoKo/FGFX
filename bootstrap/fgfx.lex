###############################################
# Lexical Specification of both (FGFL & FGFP) #
###############################################

@LETTER		=	[a-zA-Z_] ;
@DIGIT		=	[0-9] ;
@IDENT		=	{LETTER}({LETTER}|{DIGIT})* ;

## FGFL ##
G_IDENT	=	 {IDENT} ;
L_IDENT	=	@{IDENT} ;
EQUAL	=	= ;
IGCASE	=	%igcase ;
SKIP	=	%skip ;

## FGFP ##
TER		=	{IDENT} ;
NON_TER		=	{IDENT}'* ;
ENON_TER	=	<{NON_TER}> ;
ARROW		=	-> ;
UNION		=	\| ;
## TODO START symbol
#START		=	%start ;

## Both ##
SEMI		=	\; ;
COMMA		=	, ;

SPACE		=	[ \t\n]+ ;
COM		=	(#.*)|(/\*(\*[^\\]|[^*])*\*/) ;

%skip		SPACE, COM ;

