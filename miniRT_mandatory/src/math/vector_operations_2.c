/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_operations_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:47:44 by mmasarov          #+#    #+#             */
/*   Updated: 2024/10/29 16:47:45 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vector	vector_sub(t_vector v1, t_vector v2)
{
	return ((t_vector){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z});
}

t_vector	vector_add(t_vector v1, t_vector v2)
{
	return ((t_vector){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z});
}

t_vector	vector_scale(t_vector v, double scalar)
{
	return ((t_vector){v.x * scalar, v.y * scalar, v.z * scalar});
}

double	vector_dot(t_vector v1, t_vector v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

double	vector_length(t_vector v)
{
	return (sqrt(vector_dot(v, v)));
}
