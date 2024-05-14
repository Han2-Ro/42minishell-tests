/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_funcheck.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aprevrha <aprevrha@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 15:20:21 by hrother           #+#    #+#             */
/*   Updated: 2024/05/15 00:11:36 by aprevrha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../test.h"

int	main(int argc, char **argv, char **envp)
{
	t_list	*token_lst;
	t_list	*cmd_lst;
	char	*line;
	t_env	*env;
	t_evars	evars;
	char	*key;
	char	*value;

	cmd_lst = NULL;
	(void)argc;
	(void)argv;
	evars.status = 0;
	line = "\"echo \"";
	evars.envl = envp_to_list(envp);
	if (!evars.envl)
		return (1);
	key = ft_strdup("a");
	value = ft_strdup("  ");
	env = new_env(key, value);
	if (env == NULL)
		return (ft_lstclear(&evars.envl, free_env), 1);
	ft_lstadd_back(&evars.envl, ft_lstnew(env));
	token_lst = lexer(line, &evars.status);
	expand_token_list(&token_lst, evars);
	ft_lstiter(token_lst, print_token_new);
	if (token_lst)
	{
		cmd_lst = parse(token_lst);
		ft_lstiter(cmd_lst, print_cmd);
	}
	ft_lstclear(&evars.envl, free_env);
	ft_lstclear(&token_lst, free_token);
	ft_lstclear(&cmd_lst, free_cmd);
}
