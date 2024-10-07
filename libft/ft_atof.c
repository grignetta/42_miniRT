/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 16:14:43 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/01 12:09:24 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

double	return_float_part(char *s, double float_part)
{
	double	d;

	d = 1;
	while (ft_isdigit(*s))
	{
        d /= 10.0;
        float_part += (*s++ - '0') * d;
	}
	return (float_part);
}

double	ft_atof(char *s)
{
	long	int_part;
	double	float_part;
	int		minus;

	int_part = 0.0;
	float_part = 0.0;
	minus = 1;
	while ((*s >= 9 && *s <= 13) || 32 == *s)
		s++;
	if (*s == '+' || *s == '-')
		if (*s++ == '-')
			minus = -1;
	while (ft_isdigit(*s))
	{
		if (int_part > (LLONG_MAX - (*s - '0')) / 10)
			return (DBL_MAX);
		if (int_part < (LLONG_MIN + (*s - '0')) / 10)
			return (DBL_MIN);
		int_part = int_part * 10 + (*s++ - '0');
	}
	if (*s == '.')
		++s;
	float_part = return_float_part(s, float_part);
	return ((int_part + float_part) * minus);
}
