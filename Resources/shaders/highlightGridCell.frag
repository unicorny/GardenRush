#ifdef GL_ES
precision lowp float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform float border_size;
//uniform vec3 highlight_color;

void main()
{
	vec4 texColor = v_fragmentColor;// * texture2D(CC_Texture0, v_texCoord);
    //gl_FragColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord);
	float borderStrengthHeight = max(v_texCoord.x, v_texCoord.y);
	float borderStrengthLow = 1.0 - min(v_texCoord.x, v_texCoord.y);
	float borderStrength = max(borderStrengthHeight, borderStrengthLow);
	//float factor = pow(borderStrength, border_size) / pow(1.0, border_size);
	float factor = pow(borderStrength, border_size);
	//gl_FragColor += vec4(border_color, 0.2);
	//factor *= (CC_SinTime[3]+1.0)*0.5;
	//float glowFactor = (((v_texCoord.x + v_texCoord.y) + (CC_SinTime[3]+1.0)*0.5) / 3.0)* factor;
	//float glowFactor = ((v_texCoord.x + v_texCoord.y + 2.0*v_texCoord.x + (1.0-v_texCoord.y)*3.0)-1.0)*0.2*factor * (((CC_SinTime[3]+1.0)*0.5) / 3.0);
	//float glowFactor = 0.0;
	//gl_FragColor = texColor * (1.0 - factor) + (factor - glowFactor) * border_color + (glowFactor - factor) * vec4(highlight_color, 0.5);
	gl_FragColor = vec4(v_fragmentColor.xyz, factor * v_fragmentColor.a);
	//gl_FragColor = vec4(v_fragmentColor.xyz, 0.5);
}