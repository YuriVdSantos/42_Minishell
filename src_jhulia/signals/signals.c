/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhualves <jhualves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 22:21:53 by jhualves          #+#    #+#             */
/*   Updated: 2025/05/19 15:06:09 by jhualves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <signal.h>

//ARRUMAR ISSO AQUI

void	setup_signals(t_context *ctx)
{
	if (ctx->is_interactive)
		signal(SIGINT, sigint_handler);
	else
		signal(SIGINT, exit_shell);
	signal(SIGQUIT, SIG_IGN);
}

void	exit_shell(int sig)
{
	(void)sig;
	exit(EXIT_FAILURE);
}
