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
uniform float  uTimer;



 

varying vec4 varVertexColor;
varying vec4 varTexCoord;

vec4 gColor;

/* shader variables */




void main(void)
{
  
  float speed = 1.0*length(vec2(inVertexNormal.x,inVertexNormal.y));
                
          
  vec4 rotpos = 
      vec4 
      ( inVertexPosition.x*             (inVertexNormal.z>0.0?0.25:0.2),
        inVertexPosition.y*(inVertexNormal.z> 0.0
										?0.3 + speed* 0.5 
										:0.2 + speed* 0.3),
        0.0,
        0.0);

		
  
 
  float dir = inVertexPosition.z;           
	

  float len= length(rotpos);


  dir = dir +  ((uTimer*speed)/100.0*((inVertexNormal.z>0.0?2.0: 1.2) *360.0) )*3.1416/180.0;
 
 
  float an2 =  inVertexPosition.z*3.1416/180.0;
  an2 = an2 -  (((uTimer*speed)/100.0)*2.0*360.0)*3.1416/180.0;

  float rad2 = speed* sin(an2) * (inVertexNormal.z>0.0?0.2: 0.8 );
  
  float an3 =  inVertexPosition.z*3.1416/180.0;
  an3 = an3 -  (((uTimer*speed)/100.0)*2.0*360.0)*3.1416/180.0;  
  float rad3 = (inVertexNormal.z>0.0?0.2: 0.8 ) * cos(an3); 
  
  float raad = (inVertexNormal.z>0.0? 1.0 - rad2: 0.5 - rad2);     
  
   
  
  vec4 finalpos = vec4(raad*cos(dir),
                       raad*sin(dir),0.0,0.0);
    
  float an = 0.0*3.1416/180.0 
           + dir;
  
	
  an3 = 3.1416/180.0 - rad3*3.1416/2.0;
 	
  rotpos = vec4((rotpos.x)*cos(an3)-(rotpos.y)*sin(an3),
                (rotpos.x)*sin(an3)+(rotpos.y)*cos(an3),				
				0.0,
				0.0);
		
  rotpos = vec4(rotpos.x*cos(an)-rotpos.y*sin(an),
                 rotpos.x*sin(an)+rotpos.y*cos(an),
				0.0,
				0.0);
	
    vec4 vp =  + vec4(0.0,0.0,0.0,1.0) +rotpos  + finalpos;


	gColor = inVertexColor.bgra;
	gl_Position = uMvpMatrix * vp;
	
	
	varTexCoord = inTexCoord0;
	varVertexColor.rgb  = clamp(gColor.rgb, 0.0 , 1.0);
		 
	varVertexColor.a = clamp( (1.0-rad3)/2.0,0.3,1.0); 
	
	
	
	
	
}
