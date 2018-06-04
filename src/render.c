#include "rtv1.h"

void	render(t_main *m)
{
	int			i;
	t_obj		o;
	SDL_Color	*color;
	t_vec4f		intersect;

	i = -1;
	while (++i < OBJ)
	{
		o = m->objects[i];
		if (o->intersects(o->data, m->camera, m->ray, &intersect))
			color = o->get_color(o->data, intersect);
	}
}