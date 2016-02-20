precision lowp float;
precision lowp int;


varying vec4 varVertexColor;
varying vec4 varTexCoord; 
uniform sampler2D uTextureUnit0;
uniform sampler2D uTextureUnit1;

 

void main (void)
{
	vec4 color;
	vec4 texcolor = texture2D(uTextureUnit0, varTexCoord.xy);	
	vec4 texmeta = texture2D(uTextureUnit1, varTexCoord.xy);	
	
	color =  texcolor;
	 
	float luminance = (1.0-texmeta.r);
	float luminance2 = texmeta.r; 
	float mask =  luminance<=0.7  ?  1.0 : 0.0; 
	float mask3 =  luminance>0.7 &&  luminance<0.8 ?  1.0 : 0.0; 
	float mask2 =  luminance>0.2 &&  luminance<1.0 ?  1.0 : 0.0; 
	
	//float maskFactor = max(0.0, mask2*1.0);
	
	float lumicolor = (color.r) * 0.299 + (color.g) * 0.587 + (color.b) * 0.114;	
	
	color.r = clamp(mask3*0.44+ min(color.r,color.r*(1.0-texmeta.r))*1.0,0.1,1.0);
	color.g = clamp(mask3*0.40+ min(color.g,color.g*(1.0-texmeta.r))*1.0,0.1,1.0);
	color.b = clamp(mask3*0.35+ min(color.b,color.b*(1.0-texmeta.r))*1.0,0.1,1.0);
	
	color.a =mask2*texmeta.b*3.0 ; //+ mask*texmeta.b*2.0 ;
	gl_FragColor = color;
	
}
 
 
