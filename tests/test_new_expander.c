/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_new_expander.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrother <hrother@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 17:19:38 by hrother           #+#    #+#             */
/*   Updated: 2024/05/14 16:05:31 by hrother          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../test.h"

int	compare_token_list(t_list *actual, t_list *expected)
{
	t_token	*token_a;
	t_token	*token_b;

	log_msg(INFO, "compare lists: %p, %p", actual, expected);
	while (actual && expected)
	{
		token_a = actual->content;
		token_b = expected->content;
		log_msg(INFO, "Comparing tokens:");
		print_token_new(token_a);
		print_token_new(token_b);
		if (token_a->type != token_b->type)
			return (FAILURE);
		if (token_a->value != token_b->value && ft_strncmp(token_a->value,
				token_b->value, ft_strlen(token_a->value) + 1))
		{
			log_msg(ERROR, "expected: '%s', actual: '%s'", token_b->value,
				token_a->value);
			return (FAILURE);
		}
		actual = actual->next;
		expected = expected->next;
		log_msg(INFO, "equal");
	}
	if (actual || expected)
		return (FAILURE);
	return (SUCCESS);
}

int	test_expander(char *line, t_list *expected, char **envp)
{
	t_list	*token_lst;
	t_list	*new;
	int		result;
	t_evars	evars;

	evars.status = 0;
	evars.envl = envp_to_list(envp);
	ft_lstadd_back(&evars.envl, ft_lstnew(new_env(ft_strdup("numbers"),
				ft_strdup("123456"))));
	ft_lstadd_back(&evars.envl, ft_lstnew(new_env(ft_strdup("empty"),
				ft_strdup(""))));
	ft_lstadd_back(&evars.envl, ft_lstnew(new_env(ft_strdup("double_quote"),
				ft_strdup("aaa\"bbb"))));
	ft_lstadd_back(&evars.envl, ft_lstnew(new_env(ft_strdup("single_quote"),
				ft_strdup("aaa\'bbb"))));
	ft_lstadd_back(&evars.envl, ft_lstnew(new_env(ft_strdup("var1"),
				ft_strdup("aa $? bb"))));
	new = ft_lstnew(new_env(ft_strdup("ll"), ft_strdup("ls -l -a")));
	ft_lstadd_back(&evars.envl, new);
	log_msg(INFO, "line: '%s'", line);
	token_lst = lexer(line);
	result = expand_token_list(&token_lst, evars);
	ft_lstiter(token_lst, print_token_new);
	printf("Result: %i\n", result);
	result |= compare_token_list(token_lst, expected);
	ft_lstclear(&evars.envl, free_env);
	ft_lstclear(&token_lst, free_token);
	return (result);
}

int	test1(char **envp)
{
	t_list	*expected;
	t_token	token1;
	t_token	token2;
	int		result;

	token1.type = ARG;
	token1.value = "echo";
	token2.type = ARG;
	token2.value = "123456";
	expected = ft_lstnew(&token1);
	ft_lstadd_back(&expected, ft_lstnew(&token2));
	result = test_expander("echo $numbers", expected, envp);
	ft_lstclear(&expected, pass);
	return (result);
}

int	test2(char **envp)
{
	t_list	*expected;
	t_token	token1;
	t_token	token2;
	t_token	token3;
	t_token	token4;
	t_token	token5;
	int		result;

	token1.type = ARG;
	token1.value = "ls";
	token2.type = ARG;
	token2.value = "-l";
	token3.type = ARG;
	token3.value = "-a";
	token4.type = PIPE;
	token4.value = NULL;
	token5.type = ARG;
	token5.value = "cat";
	expected = ft_lstnew(&token1);
	ft_lstadd_back(&expected, ft_lstnew(&token2));
	ft_lstadd_back(&expected, ft_lstnew(&token3));
	ft_lstadd_back(&expected, ft_lstnew(&token4));
	ft_lstadd_back(&expected, ft_lstnew(&token5));
	result = test_expander("$ll|cat", expected, envp);
	return (result);
}

int	test3(char **envp)
{
	t_list	*expected;
	t_token	token1;
	int		result;

	token1.type = ARG;
	token1.value = "echo ";
	expected = ft_lstnew(&token1);
	result = test_expander("\"echo \"", expected, envp);
	return (result);
}

int	test_quote_in_var(char **envp)
{
	t_list	*expected;
	t_token	token1;
	t_token	token2;
	int		result;

	token1.type = ARG;
	token1.value = "echo";
	token2.type = ARG;
	token2.value = "aaa\'bbb";
	expected = ft_lstnew(&token1);
	ft_lstadd_back(&expected, ft_lstnew(&token2));
	result = test_expander("echo $single_quote", expected, envp);
	return (result);
}

int	test_non_existing_var(char **envp)
{
	t_list	*expected;
	t_token	token1;
	int		result;

	token1.type = ARG;
	token1.value = "aa";
	expected = ft_lstnew(&token1);
	result = test_expander("aa$non_existing_var", expected, envp);
	return (result);
}

int	test_norec_expansion(char **envp)
{
	t_list	*expected;
	t_token	token1;
	t_token	token2;
	t_token	token3;
	t_token	token4;
	int		result;

	token1.type = ARG;
	token1.value = "echo";
	token2.type = ARG;
	token2.value = "aa";
	token3.type = ARG;
	token3.value = "$?";
	token4.type = ARG;
	token4.value = "bb";
	expected = ft_lstnew(&token1);
	ft_lstadd_back(&expected, ft_lstnew(&token2));
	ft_lstadd_back(&expected, ft_lstnew(&token3));
	ft_lstadd_back(&expected, ft_lstnew(&token4));
	result = test_expander("echo $var1", expected, envp);
	return (result);
}

int	main(int argc, char **argv, char **envp)
{
	int result = SUCCESS;
	(void)argc;
	(void)argv;

	printf("\n-------- %s --------\n", argv[0]);
	result |= run_test("test1", test1, envp, true);
	result |= run_test("test2", test2, envp, true);
	result |= run_test("test3", test3, envp, true);
	result |= run_test("test_quote_in_var", test_quote_in_var, envp, true);
	result |= run_test("test_non_existing_var", test_non_existing_var, envp,
			true);
	result |= run_test("test_norec_expansion", test_norec_expansion, envp,
			false);
	printf("result: %d\n", result != SUCCESS);
	printf("------------ done ------------\n");
	return (result != SUCCESS);
}