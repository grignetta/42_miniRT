/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 16:14:43 by mmasarov          #+#    #+#             */
/*   Updated: 2024/10/03 16:22:32 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

double	ft_atof(char *s)
{
	long	int_part;
	double	float_part;
	double	d;
	int		minus;
	double	result;

	int_part = 0.0;
	float_part = 0.0;
	minus = 1;
	d = 1;
	while ((*s >= 9 && *s <= 13) || 32 == *s)
		s++;
	if (*s == '+' || *s == '-')
		if (*s++ == '-')
			minus = -1;
	while (ft_isdigit(*s))
	{
		if (int_part > LONG_MAX / 10 || (int_part == LONG_MAX / 10 && (*s - '0' > LONG_MAX % 10)))
		{
			if (minus == 1)
				return (DBL_MAX);
			else
				return (DBL_MIN);
		}
		int_part = int_part * 10 + (*s++ - '0');
	}
	if (*s == '.')
		++s;
	while (ft_isdigit(*s))
	{
		d /= 10;
		float_part = float_part + (*s++ - '0') * d;
	}
	result = (int_part + float_part) * minus;
	if (result > DBL_MAX)
		return (DBL_MAX);
	else if (result < DBL_MIN)
		return (DBL_MIN);
	return (result);
}
