/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:47:51 by mmasarov          #+#    #+#             */
/*   Updated: 2024/10/29 16:47:52 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_bonus.h"

void	set_camera(t_scene *scene)
{
	t_camera	*camera;

	camera = &scene->camera;
	camera->vp_size = 2 * tanh(camera->fov * M_PI / 360.0);
	camera->vp_d = 1.0;
}
