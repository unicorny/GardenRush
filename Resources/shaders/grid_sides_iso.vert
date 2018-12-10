attribute vec4 a_position;


#ifdef GL_ES
varying mediump vec3 vertex_pos;
varying mediump vec2 g_iso_pos;
#else
varying vec3 vertex_pos;
varying vec2 g_iso_pos;
#endif

void main()
{
	vertex_pos = a_position.xyz;
	// transform to iso 
	vec2 _3dToIso = vec2(a_position.x - (a_position.z * -1.0), a_position.y - (a_position.z * -1.0));
	vec4 isoPos = vec4(
		(_3dToIso.x - _3dToIso.y)  / 2.0,
		(_3dToIso.x + _3dToIso.y ) / 2.0,
		0.0,
		a_position.w
	);
	//isoPos = a_position;
	g_iso_pos = isoPos.xy;
    gl_Position = CC_MVPMatrix * isoPos;
}