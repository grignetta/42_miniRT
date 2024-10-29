/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:47:36 by mmasarov          #+#    #+#             */
/*   Updated: 2024/10/29 16:47:37 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	put_pixel(t_canvas *canvas, int x, int y, int color)
{
	char	*dst;

	dst = canvas->addr + (y * canvas->line_length + x
			* (canvas->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

void	check_limit_double(double *value, double limit)
{
	if (*value > limit)
		*value = limit;
}

void	check_limit_int(int *value, int limit)
{
	if (*value > limit)
		*value = limit;
}
