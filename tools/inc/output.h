#ifndef OUTPUT_H
#define OUTPUT_H

#include <stddef.h>
#include <stdint.h>

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
#define TYPEDEF		"typedef"

uint8_t min_size_type(size_t);
void output_include_macro(int, char const*);
void output_require_macro(int, char const*);
void output_endif(int, char const*);
void output_verbatim_file(int, char const*);

#endif /* OUTPUT_H */
