/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_handling_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:47:56 by mmasarov          #+#    #+#             */
/*   Updated: 2024/10/29 16:47:57 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

int	color_to_int(t_color local_color)
{
	int	final_red;
	int	final_green;
	int	final_blue;

	final_red = (int)(local_color.red * 255);
	final_green = (int)(local_color.green * 255);
	final_blue = (int)(local_color.blue * 255);
	check_limit_int(&final_red, 255);
	check_limit_int(&final_green, 255);
	check_limit_int(&final_blue, 255);
	return ((final_red << 16) | (final_green << 8) | final_blue);
}

t_color	color_to_double(int color_int)
{
	t_color	result;

	result.red = ((color_int >> 16) & 0xFF) / 255.0;
	result.green = ((color_int >> 8) & 0xFF) / 255.0;
	result.blue = (color_int & 0xFF) / 255.0;
	return (result);
}

void	color_and_light(t_trace *vars, t_intersect_result result)
{
	t_plane	*pl;
	t_color	checker_color;

	if (result.type == SHAPE_PLANE)
	{
		pl = (t_plane *)result.object;
		checker_color = get_plane_color(vars->p, pl);
		vars->local_color.red = checker_color.red * vars->lighting.red;
		vars->local_color.green = checker_color.green * vars->lighting.green;
		vars->local_color.blue = checker_color.blue * vars->lighting.blue;
	}
	else
	{
		vars->local_color.red = (vars->shape->red / 255.0)
			* vars->lighting.red;
		vars->local_color.green = (vars->shape->green / 255.0)
			* vars->lighting.green;
		vars->local_color.blue = (vars->shape->blue / 255.0)
			* vars->lighting.blue;
	}
}

int	final_color(t_trace *vars, int reflected_color_int)
{
	vars->reflected_color = color_to_double(reflected_color_int);
	vars->final_color.red = vars->local_color.red
		* (1 - vars->r) + vars->reflected_color.red * vars->r;
	vars->final_color.green = vars->local_color.green
		* (1 - vars->r) + vars->reflected_color.green * vars->r;
	vars->final_color.blue = vars->local_color.blue
		* (1 - vars->r) + vars->reflected_color.blue * vars->r;
	return (color_to_int(vars->final_color));
}
