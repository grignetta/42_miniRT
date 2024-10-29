/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_operations_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:47:40 by mmasarov          #+#    #+#             */
/*   Updated: 2024/10/29 16:47:41 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vector	vector_normalize(t_vector v)
{
	double	len;

	len = vector_length(v);
	return ((t_vector){v.x / len, v.y / len, v.z / len});
}

t_vector	vector_reflect(t_vector vr, t_vector n)
{
	return (vector_sub(vector_scale(n, 2 * vector_dot(n, vr)), vr));
}

t_vector	compute_base(t_cylinder *cyl)
{
	return (vector_sub(cyl->center,
			vector_scale(cyl->axis, cyl->height / 2.0)));
}

t_vector	compute_top(t_cylinder *cyl)
{
	return (vector_add(cyl->center,
			vector_scale(cyl->axis, cyl->height / 2.0)));
}
