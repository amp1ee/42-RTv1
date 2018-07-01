#include "rtv1.h"

void				set_pixel(t_main *m, int x, int y, unsigned int p)
{
	unsigned char	*pix;
	unsigned char	b;

	if (x < W && x >= 0 && y < H && y >= 0)
	{
		pix = (unsigned char *)m->screen->pixels;
		b = -1;
		while (++b < m->bpp)
			pix[m->bpp * (y * m->screen->w + x) + b] = (p >> (b << 3)) & 0xFF;
	}
}

static inline t_vec3f
rot_vec_x(const t_vec3f p,
				const float sin_al,
				const float cos_al) {
	const float y = p.y * cos_al - p.z * sin_al;
	const float z = p.y * sin_al + p.z * cos_al;
	return (t_vec3f){p.x, y, z};
}

static inline t_vec3f
rot_vec_y(const t_vec3f p,
				const float sin_al,
				const float cos_al) {
	const float x = p.x * cos_al - p.z * sin_al;
	const float z = p.x * sin_al + p.z * cos_al;
	return (t_vec3f){x, p.y, z};
}

static inline t_vec3f
rot_vec_z(const t_vec3f p,
				const float sin_al,
				const float cos_al) {
	const float x = p.x * cos_al - p.y * sin_al;
	const float y = p.x * sin_al + p.y * cos_al;
	return (t_vec3f){x, y, p.z};
}

static inline t_vec3f
reflect(const t_vec3f i, const t_vec3f n) 
{
	t_vec3f mi = (t_vec3f){
		-i.x,
		-i.y,
		-i.z
	};
	float idotn = vec3f_dotprod(mi, n);
	return ((t_vec3f){
		mi.x - 2 * idotn * n.x,
		mi.y - 2 * idotn * n.y,
		mi.z - 2 * idotn * n.z,
	});
}

/*
default: 
    { 
//	We use the Phong illumation model int the default case. The phong model is composed of a diffuse and a specular reflection component. 
    Vec3f lightAmt = 0, specularColor = 0; 
    Vec3f shadowPointOrig = (dotProduct(dir, N) < 0) ? 
        hitPoint + N * options.bias : 
        hitPoint - N * options.bias; 
//	Loop over all lights in the scene and sum their contribution up We also apply the lambert cosine law here though we haven't explained yet what this means. 
        for (uint32_t i = 0; i < lights.size(); ++i) { 
    	    Vec3f lightDir = lights[i]->position - hitPoint; 
    	// square of the distance between hitPoint and the light
            float lightDistance2 = dotProduct(lightDir, lightDir); 
            lightDir = normalize(lightDir); 
        	float LdotN = std::max(0.f, dotProduct(lightDir, N)); 
            Object *shadowHitObject = nullptr; 
            float tNearShadow = kInfinity; 
            // is the point in shadow, and is the nearest occluding object 
            // closer to the object than the light itself?
            bool inShadow = trace(shadowPointOrig, lightDir, objects,
            	tNearShadow, index, uv, &shadowHitObject) && 
                tNearShadow * tNearShadow < lightDistance2; 
            lightAmt += (1 - inShadow) * lights[i]->intensity * LdotN; 
            Vec3f reflectionDirection = reflect(-lightDir, N); 
            specularColor += powf(std::max(0.f, -dotProduct(reflectionDirection,
            dir)), hitObject->specularExponent) * lights[i]->intensity;
        } 
        hitColor = lightAmt * hitObject->evalDiffuseColor(st) * hitObject->Kd +
        	specularColor * hitObject->Ks; 
        break; 
    }
*/

unsigned int		trace(t_main *m, t_vec3f ray)
{
	t_vec3f 		r_vec;
	SDL_Color		*color;
	t_obj			*o;
	int				i;
	t_vec3f			intersect;
	const t_cam		*cam = m->cam;

	vec3f_normalize(&ray);
	r_vec = rot_vec_x(ray, cam->xsin, cam->xcos);
	r_vec = rot_vec_z(r_vec, cam->zsin, cam->zcos);
	r_vec = rot_vec_y(r_vec, cam->ysin, cam->ycos);
	color = &(SDL_Color){255,192,128,0};
	i = 0;
	while (i < OBJ)
	{
		o = m->objects[i];
		if (o->intersects(o->data, *m->cam->loc, r_vec, &intersect))
		{
			t_vec3f norm = o->normal_vec(o->data, intersect);
			t_vec3f lightamt = {0};
			t_vec3f spec_color = {0};
			float shadowdir = (vec3f_dotprod(r_vec, norm) < 0) ?
				EPSILON : -EPSILON;
			t_vec3f shadowpoint = (t_vec3f){
				intersect.x + norm.x * shadowdir,
				intersect.y + norm.y * shadowdir,
				intersect.z + norm.z * shadowdir
			};
			int j = 0;
			while (j < LIGHT)
			{
				t_vec3f light_dir = get_vec3f(intersect, *m->lights[j]->loc);
				float light_dist2 = vec3f_dotprod(light_dir, light_dir);
				vec3f_normalize(&light_dir);
				float ldotn = MAX(0.0f, vec3f_dotprod(light_dir, norm));
				t_obj *shadowhitobj = NULL;
				float tnearshadow = FLT_MAX;
				bool inshadow = o->intersects(o->data, shadowpoint, light_dir,
					&intersect);
				lightamt += (1 - inshadow) * m->lights[j]->brightness * ldotn;
				t_vec3f reflectdir = reflect(light_dir, norm);
				j++;
			}
			color = o->get_color(o->data, intersect);
		}
		i++;
	}
	return (color->r << 16 | color->g << 8 | color->b);
}

void				render(t_main *m)
{
	int				i, j;
	float			x, y;
	unsigned int	rgb;
	//SDL_Color		*color;

	SDL_FillRect(m->screen, NULL, 0x000000);
	i = 0;
	while (i < W)
	{
		j = 0;
		while (j < H)
		{
			x = (i - (W + m->cam->loc->x) / 2.0);
			y = (j - (H + m->cam->loc->y) / 2.0);
			t_vec3f ray = (t_vec3f){x, y, 320};
			rgb = trace(m, ray);
			set_pixel(m, i, j, rgb);
			j++;
		}
		i++;
	}
	SDL_UpdateWindowSurface(m->window);
}