/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_exec_cmd_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrother <hrother@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 21:46:55 by hrother           #+#    #+#             */
/*   Updated: 2024/04/16 16:18:49by hrother          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../test.h"

// Moved here because these tests still use some of this old code
/**
 * @brief Create a new list element
 * @deprecated use ft_lstnew instead
 */
t_list	*ft_lstnew_old(t_cmd *cmd)
{
	t_list	*new;

	new = (t_list *)malloc(sizeof(t_list));
	if (new == NULL)
		return (NULL);
	new->content = cmd;
	new->next = NULL;
	return (new);
}

/**
 * @deprecated use ft_lstadd_back instead
 */
t_list	*ft_lstadd(t_list **lst, t_cmd *cmd)
{
	t_list	*new;
	t_list	*tmp;

	tmp = *lst;
	new = ft_lstnew_old(cmd);
	if (new == NULL)
		return (NULL);
	if (tmp == NULL)
	{
		*lst = new;
		return (*lst);
	}
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new;
	return (*lst);
}

/**
 * @deprecated use ft_lstclear instead
 */
void	destroy_list(t_list *lst)
{
	t_list	*tmp;

	log_msg(DEBUG, "destroying list");
	while (lst != NULL)
	{
		tmp = lst;
		lst = lst->next;
		free(tmp->content);
		free(tmp);
	}
}

int	test_1cmd(char **envp)
{
	t_list	*cmd_list;
	t_cmd	*cmd;
	t_list	*envp_list;
	t_token	redirect;
	char	**args;
	t_evars	evars;

	envp_list = envp_to_list(envp);
	if (envp_list == NULL)
		return (FAILURE);
	evars.envl = envp_list;
	evars.tty = 0;
	log_msg(WARNING, "This test needs manual inspection of the output");
	cmd_list = NULL;
	args = malloc(sizeof(char *) * 3);
	if (args == NULL)
		return (ft_lstclear(&envp_list, free_env), FAILURE);
	args[0] = "ls";
	args[1] = "-l";
	args[2] = NULL;
	cmd = new_cmd(ft_strdup("/bin/ls"), args);
	if (cmd == NULL)
		return (ft_lstclear(&envp_list, free_env), free(args), FAILURE);
	redirect.type = R_OUT;
	redirect.value = "tests/files/out_02";
	ft_lstadd_back(&cmd->redirects, ft_lstnew(&redirect));
	if (cmd->redirects == NULL)
		return (ft_lstclear(&envp_list, free_env), free_cmd(cmd), FAILURE);
	cmd_list = ft_lstadd(&cmd_list, cmd);
	if (cmd_list == NULL)
		return (ft_lstclear(&envp_list, free_env), free_cmd(cmd), FAILURE);
	ft_lstiter(cmd_list, print_cmd);
	exec_cmd_list(cmd_list, &evars);
	ft_lstclear(&envp_list, free_env);
	return (SUCCESS);
}

int	test_2cmds(char **envp)
{
	t_list	*cmd_list;
	t_list	*envp_list;
	char	**args1;
	char	**args2;
	t_evars	evars;

	envp_list = envp_to_list(envp);
	if (envp_list == NULL)
		return (FAILURE);
	evars.envl = envp_list;
	evars.tty = 0;
	log_msg(WARNING, "This test needs manual inspection of the output");
	cmd_list = NULL;
	args1 = malloc(sizeof(char *) * 3);
	args1[0] = "ls";
	args1[1] = "-l";
	args1[2] = NULL;
	cmd_list = ft_lstadd(&cmd_list, new_cmd(ft_strdup("/bin/ls"), args1));
	args2 = malloc(sizeof(char *) * 3);
	args2[0] = "grep";
	args2[1] = "d";
	args2[2] = NULL;
	cmd_list = ft_lstadd(&cmd_list, new_cmd(ft_strdup("/bin/grep"), args2));
	if (!cmd_list || !cmd_list->next || !args1 || !args2)
		return (ft_lstclear(&envp_list, free_env), ft_lstclear(&cmd_list,
				free_cmd), FAILURE);
	ft_lstiter(cmd_list, print_cmd);
	exec_cmd_list(cmd_list, &evars);
	ft_lstclear(&envp_list, free_env);
	return (SUCCESS);
}

int	test_3cmds(char **envp)
{
	t_list	*cmd_list;
	int		result;
	t_list	*envp_list;
	char	**args1;
	char	**args2;
	char	**args3;
	t_evars	evars;

	envp_list = envp_to_list(envp);
	if (envp_list == NULL)
		return (FAILURE);
	evars.envl = envp_list;
	evars.tty = 0;
	log_msg(WARNING, "This test needs manual inspection of the output");
	cmd_list = NULL;
	args1 = malloc(sizeof(char *) * 3);
	args1[0] = "ls";
	args1[1] = "-l";
	args1[2] = NULL;
	cmd_list = ft_lstadd(&cmd_list, new_cmd(ft_strdup("ls"), args1));
	args2 = malloc(sizeof(char *) * 3);
	args2[0] = "grep";
	args2[1] = "d";
	args2[2] = NULL;
	cmd_list = ft_lstadd(&cmd_list, new_cmd(ft_strdup("grep"), args2));
	args3 = malloc(sizeof(char *) * 3);
	args3[0] = "wc";
	args3[1] = "-l";
	args3[2] = NULL;
	cmd_list = ft_lstadd(&cmd_list, new_cmd(ft_strdup("wc"), args3));
	ft_lstiter(cmd_list, print_cmd);
	result = exec_cmd_list(cmd_list, &evars);
	ft_lstclear(&envp_list, free_env);
	return (result);
}

int	test_invalid_cmd(char **envp)
{
	t_list	*envp_list;
	t_list	*cmd_list;
	char	**args;
	int		result;
	t_evars	evars;

	envp_list = envp_to_list(envp);
	if (envp_list == NULL)
		return (FAILURE);
	evars.envl = envp_list;
	evars.tty = 0;
	log_msg(WARNING, "This test needs manual inspection of the output");
	cmd_list = NULL;
	args = malloc(sizeof(char *) * 3);
	args[0] = "invalid_cmd";
	args[1] = NULL;
	cmd_list = ft_lstadd(&cmd_list, new_cmd(ft_strdup("invalid_cmd"), args));
	ft_lstiter(cmd_list, print_cmd);
	result = exec_cmd_list(cmd_list, &evars);
	ft_lstclear(&envp_list, free_env);
	if (evars.status != 127)
		result = FAILURE;
	return (result);
}

int	test_noperm_file(char **envp)
{
	t_list	*envp_list;
	t_list	*cmd_list;
	char	**args;
	int		result;
	t_token	redirect;
	t_cmd	*cmd;
	t_evars	evars;

	envp_list = envp_to_list(envp);
	if (envp_list == NULL)
		return (FAILURE);
	evars.envl = envp_list;
	evars.tty = 0;
	log_msg(WARNING, "This test needs manual inspection of the output");
	cmd_list = NULL;
	args = malloc(sizeof(char *) * 3);
	args[0] = "cat";
	args[1] = NULL;
	cmd = new_cmd(ft_strdup("cat"), args);
	redirect.type = R_IN;
	redirect.value = "tests/files/no_perm";
	ft_lstadd_back(&cmd->redirects, ft_lstnew(&redirect));
	cmd_list = ft_lstadd(&cmd_list, cmd);
	ft_lstiter(cmd_list, print_cmd);
	result = exec_cmd_list(cmd_list, &evars);
	ft_lstclear(&envp_list, free_env);
	log_msg(INFO, "status: %d", evars.status);
	if (evars.status != 1)
		result = FAILURE;
	return (result);
}

int	main(int argc, char **argv, char **envp)
{
	int result = SUCCESS;
	(void)argc;
	(void)argv;

	printf("\n-------- %s --------\n", argv[0]);
	result |= run_test("test_1cmd", test_1cmd, envp, false);
	result |= run_test("test_2cmds", test_2cmds, envp, true);
	result |= run_test("test_3cmds", test_3cmds, envp, true);
	result |= run_test("test_invalid_cmd", test_invalid_cmd, envp, true);
	result |= run_test("test_noperm_file", test_noperm_file, envp, true);
	printf("result: %d\n", result != SUCCESS);
	printf("------------ done ------------\n");
	return (result != SUCCESS);
}