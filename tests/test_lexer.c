/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_lexer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrother <hrother@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 17:19:38 by hrother           #+#    #+#             */
/*   Updated: 2024/05/13 13:45:16 by hrother          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../test.h"

// int check(char *line, )

int	test_lexer(char **envp)
{
	t_list	*token_lst;
	char	*line;

	log_msg(WARNING, "This test needs manual inspection of the output");
	(void)envp;
	line = "echo www|cat>out>>END";
	token_lst = lexer(line);
	ft_lstiter(token_lst, print_token_new);
	ft_lstclear(&token_lst, free_token);
	return (SUCCESS);
}

int	test_lexer2(char **envp)
{
	t_list	*token_lst;
	char	*line;

	log_msg(WARNING, "This test needs manual inspection of the output");
	(void)envp;
	line = "ls>out";
	token_lst = lexer(line);
	ft_lstiter(token_lst, print_token_new);
	if (ft_lstsize(token_lst) != 2)
	{
		log_msg(ERROR, "Wrong number of tokens");
		ft_lstclear(&token_lst, free_token);
		return (FAILURE);
	}
	ft_lstclear(&token_lst, free_token);
	return (SUCCESS);
}
int	main(int argc, char **argv, char **envp)
{
	int result = SUCCESS;
	(void)argc;
	(void)argv;

	printf("\n-------- %s --------\n", argv[0]);
	result |= run_test("test lexer", test_lexer, envp , true);
	result |= run_test("test lexer2", test_lexer2, envp , true);
	printf("result: %d\n", result != SUCCESS);
	printf("------------ done ------------\n");
	return (result != SUCCESS);
}