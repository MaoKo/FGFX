#################################
# Lexical Specification of FGFL #
#################################

@DIGIT	=	[0-9] ; @LETTER	=	[A-Za-z_] ;

BEG_REG	=	[^ \t\n;] ;
G_IDENT	=	{LETTER}({LETTER}|{DIGIT})* ;
L_IDENT	=	@{G_IDENT} ;
EQUAL	=	= ;
COM	=	#.* ;
SPACE	=	[ \t\n]+ ;
SEMI	=	\; ;
