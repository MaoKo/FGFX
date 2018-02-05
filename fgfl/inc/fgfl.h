#ifndef FGFL_H
#define FGFL_H

#include <stddef.h>
#include <stdbool.h>

#include "vector.h"
#include "bitset.h"
#include "buffer.h"
#include "lexer.h"
#include "utils.h"

typedef struct regex_node_t {
    enum {
        AST_UNION,
        AST_CONCAT,
        AST_QUES,
        AST_STAR,
        AST_PLUS,
        AST_LOOK,
        AST_TILDE,
        AST_OPTION,
        AST_BOUND_NAME,
        AST_DOT,
        AST_SYMBOL,
        AST_EPSILON,
        AST_CLASS,
    } kind_ast;
    union {
        struct { // if AST_UNION | AST_CONCAT | AST_LOOk
                 // | AST_QUES | AST_STAR | AST_PLUS | AST_OPTION
            struct { // if AST_OPTION
                bool igcase;
                bool dotall;
                bool skipws;
                bool reverse;
            };
            struct regex_node_t* left;
            struct regex_node_t* right;
        };
        struct { // if AST_BOUND_NAME
            char const* bound_name;
            size_t index_token;
        };
        struct // if AST_DOT
            { bool is_dotall; };
        struct { // if AST_SYMBOL
            int symbol;
            bool in_string;
        };
        struct { // if AST_CLASS
            bitset_t* class;
            int cce_lower;
            int cce_upper;
        };
    };
} regex_node_t;

typedef struct nfa_automaton_t nfa_automaton_t;

typedef struct nfa_state_t {
    size_t index;
    int final_type;
    bool is_anchor;
    bool beg_lookahead;
    bool in_look_machine;
    int symbol_edge;
    struct nfa_state_t* out_state;
    nfa_automaton_t* edge;
    union {
        nfa_automaton_t* edge2;
        bitset_t* class;
    };
    bitset_t* eclos;
} nfa_state_t;

struct nfa_automaton_t {
    size_t index; // for NFA_MASTER
    enum {
        NFA_MASTER,
        NFA_CLOSURE,
        NFA_CONCAT,
        NFA_UNION,
        NFA_SYMBOL,
        NFA_EPSILON,
        NFA_CLASS,
    } kind_nfa;
    union {
        struct // if NFA_SYMBOL
            { int out_symbol; };
        struct { // if NFA_CONCAT | NFA_UNION | NFA_CLASS
            struct // if NFA_UNION | NFA_CLASS
                { nfa_state_t* tail_state; };
            struct { // if NFA_CONCAT | NFA_UNION
                nfa_automaton_t* left;
                nfa_automaton_t* right;
            };
        };
        struct // if NFA_CLOSURE
            { nfa_automaton_t* middle; };
        struct // if NFA_MASTER
            { bitset_t* out_automata; };
    };
    nfa_state_t* head_state;
};

typedef struct {
    size_t index;
    bitset_t* set_state;
    size_t hash_state;
    trans_list_t* trans;
    bool must_ahead;
    bool must_reload;
    bool is_final;
    int final_entry;
    int final_anchor_entry;
    int group;
} dfa_state_t;

typedef struct {
    size_t index;
    int kind;
    bool is_used;
    char* name;
    union {
        struct { // if TERMINAL
            bool fragment;
            bool beg_line;
            bool use_look;
            bool must_skip;

            regex_node_t* regex_ast;
            nfa_automaton_t* nfa_m;

            int default_action;
            bool all_state;
            trans_list_t* state_lst;
        };
        struct { // if KEYWORD & STATE
            struct { // if STATE
                nfa_state_t* state_master;
                bool is_defined;
                bool is_reach;
            };
            size_t count;
        };
    };
} spec_entry_t;

typedef struct {
    lexer_t* lex;
    vector_t* entry_vect;
    vector_t* state_vect;
    int start_state;
    bool miss_regex;
    nfa_state_t* master;
    vector_t* states;
    size_t size_final;
    size_t size_final_anchor;
} lexical_spec_t;

#endif /* FGFL_H */
