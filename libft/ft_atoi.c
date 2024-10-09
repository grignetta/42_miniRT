/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 16:14:43 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/01 12:09:24 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	move_spaces(char **nptr, int *minus)
{
	while (**nptr && (**nptr == 32 || (**nptr >= 9 && **nptr <= 13)))
		(*nptr)++;
	if (**nptr == '-')
		(*minus)++;
	if (**nptr == '-' || **nptr == '+')
		(*nptr)++;
	while (**nptr == '0')
		(*nptr)++;
}

int	ft_atoi(char *nptr)
{
	int				minus;
	long long int	number;

	number = 0;
	minus = 0;
	move_spaces(&nptr, &minus);
	while (*nptr >= '0' && *nptr <= '9')
	{
		if (number > (LLONG_MAX - (*nptr - '0')) / 10)
			return (INT_MAX);
		if (number < (LLONG_MIN + (*nptr - '0')) / 10)
			return (INT_MIN);
		number = number * 10 + (*nptr - '0');
		nptr++;
	}
	if (minus == 1 && number != 0)
		return (-number);
	if (number > INT_MAX)
		return (INT_MAX);
	if (number < INT_MIN)
		return (INT_MIN);
	return (int)(number);
}
