/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_computation_utils_bonus.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:48:39 by mmasarov          #+#    #+#             */
/*   Updated: 2024/10/29 16:48:40 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

void	add_light(t_color *result, t_light light, double intensity)
{
	result->red += intensity * light.red / 255.0;
	result->green += intensity * light.green / 255.0;
	result->blue += intensity * light.blue / 255.0;
}

void	ambient_light(t_color *result, t_light light)
{
	add_light(result, light, light.intensity);
}

t_vector	get_light_direction(t_light light, t_trace vars, double *t_max)
{
	t_vector	l;

	if (light.type == 1)
	{
		l = vector_sub(light.position, vars.p);
		*t_max = vector_length(l);
		l = vector_normalize(l);
	}
	else
	{
		l = light.direction;
		*t_max = INFINITY;
		l = vector_normalize(l);
	}
	return (l);
}
