#ifndef DISPLAY_H
#define DISPLAY_H

#include <stddef.h>
#include <stdint.h>

// Format Character
#define TAB		"\t"
#define NL		"\n"
#define SP		" "
// Keywords
#define STATIC		"static"
#define VOID		"void"
#define INT		"int"
#define ENUM		"enum"
#define TYPEDEF		"typedef"
// Keywords Predicate
#define FOR(i,c,f)	"for ("#i"; "#c"; "#f")"
#define IF(c)		"if ("#c")"
#define WHILE(c)	"while ("#c")"
#define RETURN(x)	"return ("#x")"
// Punctuations
#define BEG_BLOCK	"{"
#define END_BLOCK	"}"
#define SEMI		";"
#define COMMA		","
// Function declaration
#define FUNC(name, ...)	#name"("#__VA_ARGS__")"
// Preprocessor directive
#define INCLUDE_SYS(x)	"#include <"#x">"
#define INCLUDE(x)	"#include \""#x"\""
#define DEFINE(x,y)	"#define "#x""TAB""#y
#define UNDEF(x)	"#undef"TAB""#x
#define IFNDEF(x)	"#ifndef"TAB""#x
// Macro
#define MACRO_NTER	"TOTAL_NTER"
#define MACRO_TOKEN	"TOTAL_TOKEN"

#define HEADER_SUF	"_H"

#define BYTE		8
#define SHORT		16
#define LONG		32

uint8_t min_size_type(size_t);
void display_include_macro(int, char const*);
void display_require_macro(int, char const*);
void display_endif(int, char const*);
void display_verbatim_file(int, char const*);

#endif /* DISPLAY_H */
