precision lowp float;
precision lowp int;


varying vec4 varVertexColor;
varying vec4 varTexCoord; 
uniform sampler2D uTextureUnit0;




vec4 renderColor(void)
{
	vec4 color = varVertexColor;
	
	color.a = varVertexColor.a;
	return color;
}





void main (void)
{
	gl_FragColor = renderColor();
}
