/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_test.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrother <hrother@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 14:49:40 by hannes            #+#    #+#             */
/*   Updated: 2024/05/13 13:48:51 by hrother          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	run_test(char *name, int (*test)(char **), char **envp, bool run_in_child)
{
	int result;
	int pid;

	printf("\n------%s------\n", name);
	pid = 0;
	if (run_in_child)
		pid = fork();
	if (pid == 0)
	{
		result = test(envp);
		if (run_in_child)
			exit(result);
	}
	if (run_in_child)
	{
		waitpid(pid, &result, 0);
		if (WIFEXITED(result) && WEXITSTATUS(result) == 0)
		{
			printf(GREEN "SUCCESS\n" RESET_COLOR);
			return (SUCCESS);
		}
		else
		{
			printf(RED "FAILURE\n" RESET_COLOR);
			return (FAILURE);
		}
	}
	if (result == SUCCESS)
		printf(GREEN "SUCCESS\n" RESET_COLOR);
	else
		printf(RED "FAILURE\n" RESET_COLOR);
	return (result);
}