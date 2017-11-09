#ifndef GEN_DFA_H
#define GEN_DFA_H

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
#define ENUM		"enum"
#define SEMI		";"
#define RETURN(x)	"return ("#x")"
#define COMMA		","

set_t* edges(state_t*, int);
set_t* closure(set_t**);
set_t* DFAedge(set_t*, int);
void DFAgen(token_spec_t*);

#endif /* GEN_DFA_H */
