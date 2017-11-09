#include "tokendef.h"
#include "dfa.h"

int main(int argc, char const* argv[]) {
	if (argc <= 1)
		{ exit(EXIT_FAILURE); }
	
	token_spec_t* spec = NULL;
	retreive_token_entry(argv[1], &spec);
	token_spec2nfa(spec);
	DFAgen(spec);

	return (0);
}
