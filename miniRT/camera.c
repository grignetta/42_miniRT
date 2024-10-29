/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:45:45 by mmasarov          #+#    #+#             */
/*   Updated: 2024/10/29 16:45:47 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	set_camera(t_scene *scene)
{
	t_camera	*camera;

	camera = &scene->camera;
	camera->vp_size = 2 * tanh(camera->fov * M_PI / 360.0);
	camera->vp_d = 1.0;
}
