#ifndef OUT_GEN_H
#define OUT_GEN_H

#include "utils.h"
#include "tokendef.h"

//Gen Macro
#define TAB		"\t"
#define STATIC		"static"
#define VOID		"void"
#define INT		"int"
#define BEG_BLOCK	"{"
#define END_BLOCK	"}"
#define FUNC(name, ...)	#name"("#__VA_ARGS__")"
#define FOR(i,c,f)	"for ("#i"; "#c"; "#f")"
#define IF(c)		"if ("#c")"
#define WHILE(c)	"while ("#c")"
#define ENUM		"enum"
#define SEMI		";"
#define RETURN(x)	"return ("#x")"
#define COMMA		","
#define INCLUDE_SYS(x)	"#include <"#x">"
#define INCLUDE(x)	"#include \""#x"\""
#define DEFINE(x,y)	"#define "#x""TAB""#y

int out_header(char const*, vector_t*);
int out_body(char const*, char const*, vector_t*, vector_t*);

#endif /* OUT_GEN_H */
