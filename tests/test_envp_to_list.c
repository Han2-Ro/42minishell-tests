/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_envp_to_list.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hannes <hrother@student.42vienna.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 17:19:38 by hrother           #+#    #+#             */
/*   Updated: 2024/06/09 11:28:07 by hannes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../test.h"

void	print_env(void *content)
{
	t_env	*env;

	env = (t_env *)content;
	log_msg(DEBUG, "%s=%s", env->key, env->value);
}

int	test_real(char **envp)
{
	t_list	*lst;

	lst = envp_to_list(envp);
	if (lst == NULL)
		return (FAILURE);
	printf("len: %d\n", ft_lstsize(lst));
	ft_lstiter(lst, print_env);
	ft_lstclear(&lst, free_env);
	return (SUCCESS);
}

int	main(int argc, char **argv, char **envp)
{
	int result = SUCCESS;
	(void)argc;
	(void)argv;

	printf("\n-------- %s --------\n", argv[0]);
	result |= run_test("test real", test_real, envp, true);
	printf("result: %d\n", result != SUCCESS);
	printf("------------ done ------------\n");
	return (result != SUCCESS);
}