#include "rtv1.h"

SDL_Color			lights_color(void *data, t_vec3f intersect)
{
	const t_light	*light = data;

	(void)intersect;
	return (light->color);
}

void				lights_cleanup(void *data)
{
	t_light		*lights;

	lights = data;
	free(lights);
}

t_obj				*new_light(t_vec3f *location, t_vec3f dir, double brightness, SDL_Color color)
{
	t_light		*light;
	t_obj		*obj;

	(void)dir;
	if (!(light = malloc(sizeof(*light))))
		return (NULL);
	light->loc = location;
	light->brightness = brightness;
	light->color = color;
	if (!(obj = malloc(sizeof(*obj))))
		return (NULL);
	obj->type = LIGHT_SOURCE;
	obj->data = light;
	obj->intersects = NULL;
	obj->normal_vec = NULL;
	obj->cleanup = &lights_cleanup;
	obj->get_color = &lights_color;
	return (obj);
}
