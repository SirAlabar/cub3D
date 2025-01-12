
#include <cub3d.h>

t_vector_i	vector_i_create(int x, int y)
{
	t_vector_i	v;

	v.x = x;
	v.y = y;
	return (v);
}

t_vector_i	vector_to_vector_i(t_vector v)
{
	t_vector_i	vi;

	vi.x = (int)v.x;
	vi.y = (int)v.y;
	return (vi);
}

t_vector	vector_i_to_vector(t_vector_i v)
{
	return (vector_create(v.x, v.y));
}
