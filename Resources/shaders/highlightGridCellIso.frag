#ifdef GL_ES
precision lowp float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec4 border_color;
uniform float border_size;
//uniform vec3 highlight_color;

void main()
{
    //gl_FragColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord);
	vec4 texColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord);
	
	float cx1 = (v_texCoord.x - 0.5) / -0.5;
	float cy1 = (v_texCoord.y - 0.0) /  0.5;
	float cx3 = (v_texCoord.x - 0.5) /  0.5;
	float cy3 = (v_texCoord.y - 1.0) / -0.5;
	float f1 = abs(cx1) - abs(cy1) + 1.0;
	float f2 = abs(cx3) - abs(cy3) + 1.0;
	//f1 = max(0, pow(f1, border_size) / pow(1.0, border_size));
	//f2 = max(0, pow(f2, border_size) / pow(1.0, border_size));
	f1 = max(0.0, pow(f1, border_size));
	f2 = max(0.0, pow(f2, border_size));
	float reversef1 = 1.0 - ceil(f1 - 1.1);
	float reversef2 = 1.0 - ceil(f2 - 1.1);
	//gl_FragColor = texColor * (1.0 - factor) + factor * vec4(border_color, 1.0);
	//gl_FragColor = vec4(f1, f2, 0.0, 1.0);
	//gl_FragColor = vec4(reversef1, reversef2, 0.0, 1.0);
	//gl_FragColor = texColor * (1.0 - f1 - f2) + vec4(f1 * reversef1, f2 * reversef2, 0.0, texColor.a);
	gl_FragColor = texColor * (1.0 - f1 - f2) + (f1 * reversef1 + f2 * reversef2) * vec4(border_color.xyz, texColor.a*border_color.a);
}