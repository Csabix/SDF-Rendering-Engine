vec3 color(in vec3 p, in vec3 v)
{
	return vec3(1);
}

float haz_sdf(in vec3 p, in vec3 v)
{
	float haztest = box(p + vec3(0,0,-2), v, vec3(5,4,2.5));
	float hazbelso = box(p + vec3(0,0,-2), v, vec3(4,3,2));
	float ajto = box(p + vec3(4.5,0.5,-2), v, vec3(1,1,2));
	float ablak1 = box(p + vec3(2,0,-2.8), v, vec3(0.7,10,1));
	float ablak2 = box(p + vec3(-2,0,-2.8), v, vec3(0.7,10,1));
	float ablakok = min(ablak1, ablak2);
	float haz = max(max(max(haztest, -hazbelso), -ajto), -ablakok);
	vec3 p1 = p + vec3(0,0,-4.5);
	float teto = max(sphere(p1, v, 6.0), -plane(p1, v, vec3(0,0,1)));
	return min(min(haz, teto), planeXY(p, v));
}

float sdf(in vec3 p, in vec3 v)
{
	float m = 10000;
	const int M = 6;
	for(int j=0; j < M; ++j)
	{
		const int N = 2*j+2;
		for(int i=0; i < N; ++i)
		{
			vec3 pos = vec3(cos(2*3.14*i/float(N)+j),sin(2*3.14*i/float(N)+j),0)*10*(j+2);
			m = min(m, haz_sdf(p - pos,v));
		}
	}
	return m;
}