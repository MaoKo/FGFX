$TOKEN
{
	DIGIT		=	/[0-9]/	 -> { $FRAG };
	SIGN		=	/[-+]/	 -> { $FRAG };

	F_S			=	/[flFL]/ -> { $FRAG };

	SCF_EXP		=	/[eE]{SIGN}?{DIGIT}+/ -> { $FRAG };
	BIN_EXP		=	/[pP]{SIGN}?{DIGIT}+/ -> { $FRAG };

	D_FRACT		=	/({DIGIT}+\.)/ -> { $FRAG };

	DECF		=	/({D_FRACT}{SCF_EXP}?{F_S}?)|\
									({DIGIT}+{SCF_EXP}{F_S}?)/ -> { $FRAG };
	FLOAT		=	/{D_FRACT}/	;
};
