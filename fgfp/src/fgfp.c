#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#include "cfg.h"

int main(int argc, char const* argv[]) {
	if (argc <= 1)
		{ exit(1); }
	int filde = open(argv[1], O_RDONLY);
	cfg_t* cfg = parse_cfg(filde);
	if (!cfg)
		{ exit(1); }

	puts("=== NON_TERMINAL ===");
	for (size_t i = 0; i < HASH_SIZE; ++i) {
		if (cfg->non_terminal[i]) {
			for (size_t j = 0; j <
					SIZE_VECTOR(cfg->non_terminal[i]); ++j)
				{ printf("%s\n", (char*)
					AT_VECTOR(cfg->non_terminal[i], j)); }
		}
	}

	puts("=== TERMINAL ===");
	for (size_t i = 0; i < HASH_SIZE; ++i) {
		if (cfg->terminal[i]) {
			for (size_t j = 0; j <
					SIZE_VECTOR(cfg->terminal[i]); ++j)
				{ printf("%s\n", (char*)
					AT_VECTOR(cfg->terminal[i], j)); }
		}
	}
	
	del_cfg(cfg);
	return (0);
}
