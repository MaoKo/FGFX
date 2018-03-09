#ifndef GEN_H
#define GEN_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Format Character
#define TAB             "\t"
#define NL              "\n"
#define SP              " "

// Keywords
#define STATIC          "static"
#define CONST           "const"
#define CHAR            "char"
// TMP INT
#define INT             "int"
#define ENUM            "enum"
#define TYPEDEF         "typedef"

// Punctuations
#define BEG_BLOCK       "{"
#define END_BLOCK       "}"
#define SEMI            ";"
#define COMMA           ","
#define STAR            "*"

// Preprocessor directive
#define INCLUDE_SYS(x)  "#include <"#x">"
#define INCLUDE(x)      "#include \""#x"\""
#define DEFINE(x,y)     "#define "#x""TAB""#y
#define UNDEF(x)        "#undef"TAB""#x
#define IFNDEF(x)       "#ifndef"TAB""#x

// Macro
#define MACRO_NTER      "TOTAL_NTER"
#define MACRO_TOKEN     "TOTAL_TOKEN"

#define HEADER_SUF      "H"
#define SEP             "_"

// Dead code
#define PRINT_IF_0(f)   dprintf(f, "#if 0" NL NL)
#define PRINT_END_IF(f) dprintf(f, "#endif" NL NL)

#define BYTE            (8)
#define SHORT           (16)
#define LONG            (32)

uint8_t min_size_type(size_t, bool);
void gen_include_macro(int, char const*);
void gen_require_macro(int, char const*);
void gen_endif(int, char const*);
void gen_verbatim_file(int, char const*);

#endif /* GEN_H */
