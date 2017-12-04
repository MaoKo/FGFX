@DIGIT		=	[0-9]	;
@SIGN		=	[-+]	;

@F_S		=	[flFL]	;

@SCF_EXP	=	[eE]{SIGN}?{DIGIT}+	;
@BIN_EXP	=	[pP]{SIGN}?{DIGIT}+	;

@D_FRACT	=	({DIGIT}+\.)	;

@DECF		=	({D_FRACT}{SCF_EXP}?{F_S}?)|({DIGIT}+{SCF_EXP}{F_S}?)	;
FLOAT		=	{D_FRACT}	;

