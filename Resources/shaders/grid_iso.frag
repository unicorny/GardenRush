#ifdef GL_ES
precision lowp float;
#endif

varying vec2 vertex_pos;

void main()
{
							// r                g                b
	gl_FragColor = vec4(vertex_pos.x + 0.5, vertex_pos.y + 0.5, 0.0, 0.5);
}