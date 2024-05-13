/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrother <hrother@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 16:33:22 by hrother           #+#    #+#             */
/*   Updated: 2024/05/13 13:45:16 by hrother          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../test.h"

int	test_null(char **envp)
{
	t_list	*tokens;
	t_list	*cmds;

	(void)envp;
	tokens = NULL;
	cmds = parse(tokens);
	ft_lstiter(cmds, print_cmd);
	if (cmds == NULL)
		return (SUCCESS);
	else
		return (FAILURE);
}

int	test_1cmd(char **envp)
{
	t_list	*tokens;
	t_list	*cmds;
	t_cmd	*cmd;
	t_token	token[3];
	int		result;

	(void)envp;
	result = SUCCESS;
	tokens = NULL;
	token[0] = (t_token){.type = ARG, .value = "echo"};
	ft_lstadd_back(&tokens, ft_lstnew(&token[0]));
	token[1] = (t_token){.type = ARG, .value = "Hello"};
	ft_lstadd_back(&tokens, ft_lstnew(&token[1]));
	token[2] = (t_token){.type = R_OUT, .value = "out.txt"};
	ft_lstadd_back(&tokens, ft_lstnew(&token[2]));
	ft_lstiter(tokens, print_token);
	cmds = parse(tokens);
	ft_lstclear(&tokens, pass);
	ft_lstiter(cmds, print_cmd);
	if (cmds == NULL)
		return (FAILURE);
	if (ft_lstsize(cmds) != 1)
		return (FAILURE);
	cmd = (t_cmd *)cmds->content;
	if (!cmd->bin || ft_strncmp(cmd->bin, "echo", 10) != 0)
		result = FAILURE;
	else if (!cmd->args || !cmd->args[0] || ft_strncmp(cmd->args[0], "echo",
			10) != 0)
		result = FAILURE;
	else if (!cmd->args || !cmd->args[1] || ft_strncmp(cmd->args[1], "Hello",
			10) != 0)
		result = FAILURE;
	else if (cmd->args[2] != NULL)
		result = FAILURE;
	else if (ft_lstsize(cmd->redirects) != 1)
		result = FAILURE;
	else if (((t_token *)cmd->redirects->content)->type != R_OUT)
		result = FAILURE;
	else if (ft_strncmp(((t_token *)cmd->redirects->content)->value, "out.txt",
			10) != 0)
		result = FAILURE;
	// TODO: cmd->pid is not checked
	ft_lstclear(&cmds, free_cmd);
	return (result);
}

int	test_3cmd(char **envp)
{
	t_list	*tokens;
	t_list	*cmds;
	t_cmd	*cmd;
	t_token	token[11];

	(void)envp;
	tokens = NULL;
	token[0] = (t_token){.type = ARG, .value = "echo"};
	ft_lstadd_back(&tokens, ft_lstnew(&token[0]));
	token[1] = (t_token){.type = ARG, .value = "Hello"};
	ft_lstadd_back(&tokens, ft_lstnew(&token[1]));
	token[2] = (t_token){.type = R_IN, .value = "in.txt"};
	ft_lstadd_back(&tokens, ft_lstnew(&token[2]));
	token[3] = (t_token){.type = PIPE, .value = NULL};
	ft_lstadd_back(&tokens, ft_lstnew(&token[3]));
	token[4] = (t_token){.type = ARG, .value = "ls"};
	ft_lstadd_back(&tokens, ft_lstnew(&token[4]));
	token[5] = (t_token){.type = ARG, .value = "-l"};
	ft_lstadd_back(&tokens, ft_lstnew(&token[5]));
	token[6] = (t_token){.type = ARG, .value = "-h"};
	ft_lstadd_back(&tokens, ft_lstnew(&token[6]));
	token[7] = (t_token){.type = PIPE, .value = NULL};
	ft_lstadd_back(&tokens, ft_lstnew(&token[7]));
	token[8] = (t_token){.type = R_OUT, .value = "out1.txt"};
	ft_lstadd_back(&tokens, ft_lstnew(&token[8]));
	token[9] = (t_token){.type = ARG, .value = "cat"};
	ft_lstadd_back(&tokens, ft_lstnew(&token[9]));
	token[10] = (t_token){.type = R_OUT, .value = "out2.txt"};
	ft_lstadd_back(&tokens, ft_lstnew(&token[10]));
	ft_lstiter(tokens, print_token);
	if (ft_lstsize(tokens) != 11)
		return (ft_lstclear(&tokens, pass), FAILURE);
	cmds = parse(tokens);
	ft_lstiter(cmds, print_cmd);
	if (cmds == NULL)
		return (ft_lstclear(&tokens, pass), FAILURE);
	if (ft_lstsize(cmds) != 3)
		return (FAILURE);
	cmd = (t_cmd *)cmds->content;
	if (!cmd->bin || ft_strncmp(cmd->bin, "echo", 10) != 0)
		return (FAILURE);
	if (!cmd->args || !cmd->args[0] || ft_strncmp(cmd->args[0], "echo",
			10) != 0)
		return (ft_lstclear(&tokens, pass), ft_lstclear(&cmds, free_cmd),
			FAILURE);
	if (!cmd->args[1] || ft_strncmp(cmd->args[1], "Hello", 10) != 0)
		return (ft_lstclear(&tokens, pass), ft_lstclear(&cmds, free_cmd),
			FAILURE);
	if (cmd->args[2] != NULL)
		return (FAILURE);
	if (ft_lstsize(cmd->redirects) != 1)
		return (FAILURE);
	if (ft_strncmp(((t_token *)cmd->redirects->content)->value, "in.txt",
			10) != 0)
		return (FAILURE);
	cmd = (t_cmd *)cmds->next->content;
	if (!cmd->bin || ft_strncmp(cmd->bin, "ls", 10) != 0)
		return (FAILURE);
	if (!cmd->args || !cmd->args[0] || !cmd->args[1] || !cmd->args[2])
		return (FAILURE);
	if (ft_strncmp(cmd->args[0], "ls", 10) != 0)
		return (FAILURE);
	if (ft_strncmp(cmd->args[1], "-l", 10) != 0)
		return (FAILURE);
	if (ft_strncmp(cmd->args[2], "-h", 10) != 0)
		return (FAILURE);
	if (cmd->args[3] != NULL)
		return (FAILURE);
	if (ft_lstsize(cmd->redirects) != 0)
		return (FAILURE);
	cmd = (t_cmd *)cmds->next->next->content;
	if (!cmd->bin || ft_strncmp(cmd->bin, "cat", 10) != 0)
		return (FAILURE);
	if (ft_strncmp(cmd->args[0], "cat", 10) != 0)
		return (FAILURE);
	if (cmd->args[1] != NULL)
		return (FAILURE);
	if (ft_lstsize(cmd->redirects) != 2)
		return (FAILURE);
	if (ft_strncmp(((t_token *)cmd->redirects->content)->value, "out1.txt",
			10) != 0)
		return (FAILURE);
	if (ft_strncmp(((t_token *)cmd->redirects->next->content)->value,
			"out2.txt", 10) != 0)
		return (FAILURE);
	// TODO: cmd->pid is not checked
	ft_lstclear(&tokens, pass);
	ft_lstclear(&cmds, free_cmd);
	return (SUCCESS);
}

int	main(int argc, char **argv, char **envp)
{
	int	result;

	result = SUCCESS;
	(void)argc;
	(void)argv;
	printf("\n-------- %s --------\n", argv[0]);
	result |= run_test("test_1cmd", test_1cmd, envp , false);
	result |= run_test("test_3cmd", test_3cmd, envp , false);
	result |= run_test("test_null", test_null, envp , false);
	// TODO: test heredoc and append
	printf("result: %d\n", result != SUCCESS);
	printf("------------ done ------------\n");
	return (result != SUCCESS);
}
