
layout(index = 0 + 1) subroutine(SphereTrace)
vec3 spheretrace_1(const in Ray r, in float t, in float ft, const in int maxiters)
{	//pure sphere trace (low step count causes bubbley artifact)
	for(int i = 0; i < maxiters; ++i)
	{
		ft = SDF(r,t);
		if(IS_CLOSE_TO_SURFACE(ft,t)) return vec3(t, SDF(r, t-ft), float(i));
		t += ft;
	}
	return vec3(t, ft, float(maxiters));
}