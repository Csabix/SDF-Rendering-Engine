
layout(index = 0 + 1) subroutine(SphereTrace)
vec3 spheretrace_1(const in Ray r, in float t, float ft)
{	//pure sphere trace (low step count causes bubbley artifact)
	for(int i = 0; i < st_stepcount; ++i)
	{
		ft = SDF(r,t);
		if(ft < t * cam_pixel_growth*0.2) return vec3(t - ft, SDF(r, t-ft), float(i));
		t += ft;
	}
	return vec3(t, ft, float(st_stepcount));
}