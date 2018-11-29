attribute vec4 a_position;


#ifdef GL_ES
varying mediump vec2 vertex_pos;
#else
varying vec2 vertex_pos;
#endif

void main()
{
	vertex_pos = a_position.xy;
    gl_Position = CC_MVPMatrix * a_position;
}