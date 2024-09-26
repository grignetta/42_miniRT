#include "minirt.h"

void	free_scene(scene *sc)
{
	if (sc->spheres)
	{
		free(sc->spheres);
		sc->spheres = NULL;
	}
	if (sc->cylinders)
	{
		free(sc->cylinders);
		sc->cylinders = NULL;
	}
	if (sc->planes)
	{
		free(sc->planes);
		sc->planes = NULL;
	}
	if (sc->lights)
	{
		free(sc->lights);
		sc->lights = NULL;
	}
	sc->success = 1;
}
