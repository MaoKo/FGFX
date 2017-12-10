/////////////////////////////////////////////////
// Lexical Specification of both (FGFL & FGFP) //
/////////////////////////////////////////////////

/* High Precedence */

SPACE		=	/  [ \t\n]+					/ ;
COM		=	/  (\/\/.*)|(\/\*(\*+[^*\/]|[^*])*\*+\/)	/ ;

%ignore		SPACE, COM ;

@LETTER		=	/  [a-zA-Z_]			/ ;
@DIGIT		=	/  [0-9]			/ ;
@IDENT		=	/  {LETTER}({LETTER}|{DIGIT})*	/ ;

/* FGFL */

LOCAL_TOK	=	/  @{IDENT}			/ ;
EQUAL		=	/  =				/ ;
REGEX		=	/  \/([^\/\\\n]|\\(.|\n))*\/	/ ;

/* FGFP */

NTER		=	/  <{IDENT}'?>			/ ;
ARROW		=	/  ->				/ ;
UNION		=	/  \|				/ ;
EMPTY		=	/  %empty%			/ ;
LITERAL		=	/  '([^\\\n']|\\['\\])'		/ ;
PATH		=	/  \"([^\\\n"]|\\[\"])+\"	/ ;

/* Both FGFL & FGFP */

DIRECTIVE	=	/  %{IDENT}	/ ;
GLOBAL_TOK	=	/  {IDENT}	/ ;
SEMI		=	/  ;		/ ;
COMMA		=	/  ,		/ ;
LBRACK		=	/  \[		/ ;
RBRACK		=	/  ]		/ ;

%special DIRECTIVE -> [
		START, INCLUDE, DEFINE,
		IGNORE, SPECIAL, IGCASE,
	];
