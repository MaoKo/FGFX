/////////////////////////////////////////////////
// Lexical Specification of both (FGFL & FGFP) //
/////////////////////////////////////////////////

/* High Precedence */

SPACE		=	/  [ \t\n]+					/ ;
COM		=	/  (\/\/.*)|(\/\*(\*+[^*\/]|[^*])*\*+\/)	/ ;

[ignore]	SPACE, COM ;

@LETTER		=	/  [a-zA-Z_]			/ ;
@DIGIT		=	/  [0-9]			/ ;
@IDENT		=	/  {LETTER}({LETTER}|{DIGIT})*	/ ;

/* FGFL */

LOCAL_TOK	=	/  @{IDENT}			/ ;
EQUAL		=	/  =				/ ;
REGEX		=	/ \/([^\/\\\n]|\\(.|\n))*\/	/ ;

/* FGFP */

NTER		=	/  <{IDENT}'?>			/ ;
ARROW		=	/  ->				/ ;
UNION		=	/ \|				/ ;
EMPTY		=	/  %empty%			/ ;
LITERAL		=	/  '([^\n\\'"]|\\[\\'"])+'	/ ;

/* Both FGFL & FGFP */

DIRECTIVE	=	/ \[{IDENT}]	/ ;
GLOBAL_TOK	=	/  {IDENT}	/ ;
SEMI		=	/  ;		/ ;
COMMA		=	/  ,		/ ;
LBRACE		=	/ \{		/ ;
RBRACE		=	/  }		/ ;

[define] DIRECTIVE -> {
		START, EXTERN, ALIAS,
		IGNORE, DEFINE, IGCASE,
	};
