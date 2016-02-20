precision lowp float;
precision lowp int;


varying vec4 varVertexColor;
varying vec4 varTexCoord; 
uniform sampler2D uTextureUnit0;
uniform sampler2D uTextureUnit1;


/*  
vec4 tex2DBiLinear(sampler2D texunit)
{

	lowp vec2 metaTexCoord = vec2(gl_FragCoord.x/640.0, gl_FragCoord.y/480.0);
	float texelSizeX = 1.0/640.0;
	float texelSizeY = 1.0/480.0;
	 
	
    vec4 p0q0 = texture2D(texunit, metaTexCoord);
    vec4 p1q0 = texture2D(texunit, metaTexCoord + vec2(texelSizeX, 0));

    vec4 p0q1 = texture2D(texunit, metaTexCoord + vec2(0, texelSizeY));
    vec4 p1q1 = texture2D(texunit, metaTexCoord + vec2(texelSizeX , texelSizeY));
 
    float a = fract( metaTexCoord.x * 640.0 ); // Get Interpolation factor for X direction.
					// Fraction near to valid data.

    vec4 pInterp_q0 = mix( p0q0, p1q0, a ); // Interpolates top row in X direction.
    vec4 pInterp_q1 = mix( p0q1, p1q1, a ); // Interpolates bottom row in X direction.

    float b = fract( metaTexCoord.y * 480.0);// Get Interpolation factor for Y direction.
    return mix( pInterp_q0, pInterp_q1, b ); // Interpolate in Y direction.
	
}
*/

vec4 processAt(vec2 textcoord)
{
	vec4 color;
	vec4 texcolor = texture2D(uTextureUnit0, textcoord.xy);	
	vec4 texmeta = texture2D(uTextureUnit1, textcoord.xy);	
	
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
	//color.r = clamp(min(color.r,color.r*(1.0-texmeta.r))*1.0,0.1,1.0);
	//color.g = clamp(min(color.g,color.g*(1.0-texmeta.r))*1.0,0.1,1.0);
	//color.b = clamp(min(color.b,color.b*(1.0-texmeta.r))*1.0,0.1,1.0);
	//color.a =  mask * color.a; // (lumicolor*mask + mask*color.a) * 1.0;
	color.a =mask2*texmeta.b*3.0 ; //+ mask*texmeta.b*2.0 ;
	return color;
}

vec4 renderColor(void)
{ 
	float texelSizeX = 1.0/764.0;
	float texelSizeY = 1.0/542.0;
	vec2 txy =  varTexCoord.xy;
	
	vec4 p0q0 = processAt(txy);
    vec4 p1q0 = processAt(txy + vec2(texelSizeX, 0));
    vec4 p0q1 = processAt(txy + vec2(0, texelSizeY));
    vec4 p1q1 = processAt(txy + vec2(texelSizeX , texelSizeY)); 
	
	float a = fract( txy.x * 764.0 ); 	
    vec4 pInterp_q0 = mix( p0q0, p1q0, a ); // Interpolates top row in X direction.
    vec4 pInterp_q1 = mix( p0q1, p1q1, a ); // Interpolates bottom row in X direction.

    float b = fract( txy.y *542.0);// Get Interpolation factor for Y direction.
	vec4 color =   mix( pInterp_q0, pInterp_q1, b ); // Interpolate in Y direction.
	return color;
	
} 
 

void main (void)
{
	gl_FragColor = renderColor();
}
 
 
