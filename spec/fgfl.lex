#################################
# Lexical Specification of FGFL #
#################################

@DIGIT	=	[0-9] ;
@LETTER	=	[A-Za-z_] ;

G_IDENT	=	{LETTER}({LETTER}|{DIGIT})* ;
L_IDENT	=	@{G_IDENT} ;
EQUAL	=	= ;
COM	=	#.* ;
SPACE	=	[ \t\n]+ ;
SEMI	=	\; ;
IGCASE	=	%igcase ;
SKIP	=	%skip ;
COMMA	=	, ;

%skip COM, SPACE;
