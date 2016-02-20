precision lowp float;
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



 

varying vec4 varVertexColor;
varying vec4 varTexCoord;

vec4 gColor;

/* shader variables */



void main(void)
{
	gl_Position = uMvpMatrix * inVertexPosition;
	

	gColor = inVertexColor.bgra;

	varVertexColor = gColor;	
	varVertexColor.rgb  = clamp(varVertexColor.rgb, 0.0, 1.0);
	varVertexColor.a = gColor.a;
	
	
}
