#ifdef GL_ES
precision lowp float;
#endif

varying vec3 vertex_pos;
varying vec2 g_iso_pos;
uniform vec2 tex_coord_rects[4];

void main()
{
	//int indexAdd = int(ceil(vertex_pos.y+0.4999));
	int indexAdd = int(ceil(vertex_pos.y + 0.499));
	indexAdd = 1 - indexAdd;
	vec2 origin = tex_coord_rects[indexAdd * 2];
	vec2 size   = tex_coord_rects[indexAdd * 2 + 1];
	//*/
	
	float z = vertex_pos.z * -8.0;
	//float x = vertex_pos.x + 0.5 + (1.0 - ceil(vertex_pos.x)) * z;
	float x = vertex_pos.x + 0.5 + vertex_pos.y + 0.5;
	// kacheln
	x *= 4.0;
	vec2 tex_coords = vec2(
		x - floor(x), // wrap
		//vertex_pos.y + 0.5 + (1.0 - ceil(vertex_pos.y)) * z
		z
	);
	//texture2D(CC_Texture0, v_texCoord);
							// r                g                b
	//gl_FragColor = vec4(tex_coords.x, 1.0, tex_coords.y, 1.0);
	//gl_FragColor = vec4(vertex_pos.x + 0.5, 1.0, vertex_pos.y + 0.5, 1.0);
	//gl_FragColor = vec4(g_iso_pos.x, 0.0, g_iso_pos.y, 0.0);
	gl_FragColor = texture2D(CC_Texture0, tex_coords * size + origin);
	//gl_FragColor = vec4(indexAdd, 0, 0, 1.0);
	//gl_FragColor = vec4(origin.x, origin.y, size.x, size.y);
	//gl_FragColor = texture2D(CC_Texture0, vec2(vertex_pos.xy + 0.5));
}