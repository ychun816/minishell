#include "minishell.h"

int main(int ac, char *av[], char *env[])
{
    (void)ac;
	(void)av;
	t_shell *content = init_shell(env);
	if (!content) 
	{
		fprintf(stderr, "Failed to initialize shell\n");
		return (1);
	}

	/** 1. TEST: build_for_exec */



	return 0;
}