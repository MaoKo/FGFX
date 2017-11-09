#############################
## C Token Definition (C11) #
#############################

## C Identifier token

@ESC_UN		=	(\\u[0-9a-fA-F]{4}|\\U[0-9a-fA-F]{8})
@LETTER		=	([A-Za-z_])
@DIGIT		=	([0-9])
IDENT		= 	({LETTER}|{ESC_UN})({LETTER}|{ESC_UN}|{DIGIT})*

## C Char token

@ESC_OCT	=	(\\[0-7]{1,3})
@ESC_HEX	=	(\\x[0-9a-fA-F]+)
@ESC_SEQ	=	(\\'|\\\"|\\?|\\\\|\\a|\\b|\\f|\\n|\\r|\\t|\\v)
@REG_ESCAPE	=	({ESC_SEQ}|{ESC_OCT}|{ESC_HEX}|{ESC_UN})
CHAR		=	[LuU]?'([^\n'\\]|{REG_ESCAPE})+'

## C String token

STR		=	(u8?|[UL])?\"([^\n"\\]|{REG_ESCAPE})*\"

## C Punctuation

COMMA		=	,
SEMICOLON	=	;
LPAREN		=	\(
RPAREN		=	\)
LBRACK		=	\{
RBRACK		=	}
LSUB		=	\[
RSUB		=	]
QUES		=	?
ELLIP		=	\.\.\.
EXCLA		=	!

PLUS		=	\+
PLUSPLUS	=	\+\+
MINUS		=	-
MINUSMINUS	=	--
DIV		=	/
MUL		=	\*
MOD		=	%
XOR		=	^
ORB		=	\|
ANDB		=	&
ORL		=	\|\|
ANDL		=	&&
TILDE		=	~
ASSIGN		=	=

PLUSE		=	\+=
MINUSE		=	-=
DIVE		=	/=
MULE		=	\*=
MODE		=	%=
XORE		=	^=
ORBE		=	\|=
ANDBE		=	&=
ORLE		=	\|\|=
ANDLE		=	&&=
TILDEE		=	~=

EQ		=	==
NEQ		=	!=
LESS		=	<
GREAT		=	>
LESSE		=	<=
GREATE		=	>=

COLON		=	:
DASH		=	#

## C Comment

COMMENT		=	(//.*)|(/\*(\*[^/]|[^*])*\*/)

## C Number

@DEC		=	([1-9][0-9]*)
@OCT		=	(0[0-7]*)
@HEX		=	(0[xX][0-9a-fA-F]*)
@U_S		=	[uU]
@L_S		=	[lL]
@LL_S		=	(ll|LL)
@INT_S		=	(({U_S}{L_S}?)|({U_S}{LL_S})|({L_S}{U_S}?)|({LL_S}{U_S}?))
INTEGER		=	({DEC}|{OCT}|{HEX}){INT_S}?

