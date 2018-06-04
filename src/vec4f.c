#include "rtv1.h"

float		vec4f_length(t_vec4f vec)
{
	float	res;
	int		i;

	res = 0.0;
	i = 0;
	while (i < 3)
		res += pow(vec[i++], 2);
	return (sqrt(res));
}

double		vec4f_dotprod(t_vec4f a, t_vec4f b)
{
	double	prod;
	int		i;

	prod = 0.0;
	i = -1;
	while (++i < 3)
		prod += a[i] * b[i];
	return (prod);
}

t_vec4f		get_vec4f(t_vec4f p0, t_vec4f p1)
{
	t_vec4f	v;

	v = (t_vec4f){
		p1[0] - p0[0],
		p1[1] - p0[1],
		p1[2] - p0[2],
		p1[3] - p0[3]
	};
	return (v);
}

void		vec4f_normalize(t_vec4f *vec)
{
	const float	vlen = vec4f_length(*vec);
	int			i;

	i = 0;
	while (i < 3)
		(*vec)[i++] /= vlen;
}
