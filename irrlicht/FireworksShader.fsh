precision lowp float;
precision lowp int;



varying vec4 varVertexColor;
varying vec4 varTexCoord; 
varying float brightness;

uniform sampler2D uTextureUnit0;


vec4 renderColor(void)
{
	
	vec4 texel = texture2D(uTextureUnit0, varTexCoord.xy);	
	vec3 black = vec3(0.0,0.0,0.0);
	
	vec4 color;
    color.rgb = texel.a > 0.9 ? 
				clamp((texel.rgb * brightness/4.0) +  texel.a*varVertexColor.rgb, 0.4,1.0): 
				texel.rgb * varVertexColor.rgb;
		
		
	//color = vec4(mix(black,  texel.rgb * varVertexColor.rgb,2.0*texel.a), 1.0);
		
	//color.rgb =  texel.rgb * varVertexColor.rgb + brightness;
	
	
	 
	color.a = texel.a; 
	return color;
	 
}

void main (void)
{	
	gl_FragColor = renderColor();
	
		
}
