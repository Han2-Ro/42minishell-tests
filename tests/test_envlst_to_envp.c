/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_envlst_to_envp.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrother <hrother@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 17:19:38 by hrother           #+#    #+#             */
/*   Updated: 2024/05/13 13:45:18 by hrother          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../test.h"

int	test_envlst_to_envp(char **envp)
{
	t_list	*lst;
	char	**my_envp;
	int		i;

	lst = envp_to_list(envp);
	if (!lst)
		return (log_msg(WARNING, "envp_to_list() failed"), FAILURE);
	my_envp = envlst_to_envp(lst);
	if (!my_envp)
	{
		ft_lstclear(&lst, free_env);
		log_msg(WARNING, "envlst_to_envp() returned NULL");
		return (FAILURE);
	}
	i = 0;
	while (envp[i])
	{
		log_msg(DEBUG, "in, out:\n%s\n%s", my_envp[i], envp[i]);
		if (ft_strncmp(envp[i], my_envp[i], ft_strlen(envp[i])) != 0)
		 {
			ft_lstclear(&lst, free_env);
			free_nullterm_str_arr(my_envp);
			return (log_msg(WARNING, "in- and output did not match"), FAILURE);
		 }
		i++;
	}
	if (my_envp[i] != NULL)
		return (log_msg(WARNING, "nullterm wrong"), FAILURE);
	ft_lstclear(&lst, free_env);
	free_str_arr(my_envp, i);
	return (SUCCESS);
}

int	main(int argc, char **argv, char **envp)
{
	int result = SUCCESS;
	(void)argc;
	(void)argv;

	printf("\n-------- %s --------\n", argv[0]);
	result |= run_test("test envlst_to_envp", test_envlst_to_envp, envp, true);
	printf("result: %d\n", result != SUCCESS);
	printf("------------ done ------------\n");
	return (result != SUCCESS);
}