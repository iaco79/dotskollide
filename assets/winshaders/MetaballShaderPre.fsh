precision lowp float;
precision lowp int;


varying vec4 varVertexColor;
varying vec4 varTexCoord; 
uniform sampler2D uTextureUnit0;
uniform sampler2D uTextureUnit1;



vec4 renderColor(void)
{ 
	vec4 texcolor = texture2D(uTextureUnit0, varTexCoord.xy);	
	vec4 texmeta =  texture2D(uTextureUnit1, varTexCoord.xy);	
	
	
	vec4 color =   texcolor;	
	/*color.r = texmeta.r;
	color.g = texmeta.g;
	color.b = texcolor.a;
	*/
	color.r = (texmeta.r+texmeta.g+texmeta.b) /3.0;	
	color.b = (texcolor.r+texcolor.g+texcolor.b) /3.0;	
	color.g = color.b;
	color.a = 0.0;
	
	return color;
	
} 
 

void main (void)
{
	gl_FragColor = renderColor();
}
 
 
