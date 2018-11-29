attribute vec4 a_position;


#ifdef GL_ES
varying mediump vec2 vertex_pos;
#else
varying vec2 vertex_pos;
#endif

void main()
{
	vertex_pos = a_position.xy;
	// transform to iso 
	vec4 isoPos = vec4(
		(a_position.x - a_position.y) / 2.0,
		(a_position.x + a_position.y) / 2.0,
		a_position.z,
		a_position.w
	);
	//isoPos = a_position;
    gl_Position = CC_MVPMatrix * isoPos;
}