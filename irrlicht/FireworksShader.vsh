precision mediump float;
precision lowp int;


attribute vec4 inVertexPosition;
attribute vec4 inVertexColor;
attribute vec4 inTexCoord0;
attribute vec3 inVertexNormal;

//attribute vec4 inTexCoord1;


uniform int uRenderMode;

/* Matrix Uniforms */

uniform mat4 uMvpMatrix;
uniform mat4 uWorldMatrix;
uniform vec4 uFogColor;
uniform float  uTimer;


 

varying vec4 varVertexColor;
varying vec4 varTexCoord;
varying float brightness;

/* shader variables */




void main(void)
{
	vec4 gColor;
	float length =inVertexPosition.z;
	float speed = inVertexNormal.x;
	float width = inVertexNormal.y; 
	
	float widthAngle = width>0.0? 3.1416/2.0: (3.1416/2.0)*3.0;
	width = abs(width);
	
	float t= (uTimer-speed > 0.0)? (uTimer-speed) * (1.0/(1.0-speed)): 0.0;
	float timeoffset = (uTimer-speed > 0.0)?  (1.0/(1.0-speed))*inVertexNormal.z: 0.0; 

	//float t= (uTimer-speed > 0.0)? (uTimer-speed): 0.0;
	//float timeoffset = (uTimer-speed > 0.0)?  inVertexNormal.z: 0.0; 
	
	
	 
	widthAngle = atan(inVertexPosition.y, inVertexPosition.x) + widthAngle;
	
	
	
	 
	t = t + timeoffset;
	
	t= clamp(t,0.0,1.0);
	
	//float radius = sin(t*3.1416/2.0);
	float radius = (2.0-exp(-3.3+(2.0-t)*(2.0-t)))/2.0;
	radius = clamp(radius,0.001,1.0);
	
    vec4 ppos = vec4(inVertexPosition.x*radius*length,inVertexPosition.y*radius*length,0.0,1.0) ;
	vec4 pw = vec4(cos(widthAngle)*width,sin(widthAngle)*width,0.0,1.0) ;
	
	float br=  ((2.0-t)*exp(-3.3+(2.0-t)*(2.0-t)));
	
	
	br =  clamp(br,0.0,2.0);
	
	
	vec3 black = vec3(0.0,0.0,0.0);
	
	
	vec4 vp = ppos + pw;
	
	 brightness = br;

	//gColor = uFogColor.rgba;
	
	gColor = vec4(mix(black,  uFogColor.rgb, br), 1.0);
	
	gl_Position = uMvpMatrix * vp;
	
	
	varTexCoord = inTexCoord0;
	varVertexColor.rgb  = clamp(gColor.rgb, 0.0 , 1.0);
		 
	varVertexColor.a = 1.0; 
	
	
	
	
}
