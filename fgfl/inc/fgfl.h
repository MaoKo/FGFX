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
// TODO
        AST_STAR,
        AST_PLUS,
        AST_QUES,
// TODO
        AST_LOOK,
        AST_CLOSURE,
        AST_BOUND_NAME,
        AST_SYMBOL,
        AST_EPSILON,
        AST_CLASS,
    } kind_ast;
    union {
        struct { // if AST_UNION | AST_CONCAT | AST_CLOSURE
            struct regex_node_t* left;
            struct regex_node_t* right;
        };
        struct { // if AST_BOUND_NAME
            char const* bound_name;
            size_t index_token;
        };
        struct // if AST_SYMBOL
            { int symbol; };
        struct // if AST_CLASS
            { bitset_t* class; };
    };
} regex_node_t;

typedef struct nfa_automaton_t nfa_automaton_t;

typedef struct nfa_state_t {
    size_t index;
    int final_type;
    bool is_anchor;
    bool beg_look;
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
    int final_entry;
    int final_anchor_entry;
    bool middle;
    int group;
} dfa_state_t;

typedef struct {
    size_t index;
    int kind;
    bool is_used;
    char* name;
    union {
        struct { // if TERMINAL
            bool is_igcase;
            bool is_frag;

            bool use_lower;
            bool use_upper;

            bool beg_line;

            bool use_look;
            bool skip;

            regex_node_t* reg_ast;
            nfa_automaton_t* nfa_m;

            int default_state;

            bool all_state;
            trans_list_t* state_begin_lst;
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
