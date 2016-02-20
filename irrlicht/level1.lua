 gscriptable = nil
 

 co = coroutine.create(function ()
 
 
			-- INICIA script
		
			i = 1 
			
			gscriptable:CreateElementAtXY( -10, 0,0x00)
			gscriptable:CreateElementAtXY(  10, 0,0x00)			
			coroutine.yield()
			coroutine.yield()			
			
			gscriptable:CreateElementAtXY( 0,  10,0x00)
			gscriptable:CreateElementAtXY( 0, -10,0x00)
			coroutine.yield()
			coroutine.yield()			
			
			
			gscriptable:CreateElementAtXYSpeed( -40,  40,270,30,0x00)
			gscriptable:CreateElementAtXYSpeed( -40, -40,90,30,0x00)
			coroutine.yield()
			coroutine.yield()			
			
			gscriptable:CreateElementAtXYSpeed( 40,  40,270,30,0x00)
			gscriptable:CreateElementAtXYSpeed( 40, -40,90, 30,0x00)				
			coroutine.yield()
			coroutine.yield()			
			
			
			gscriptable:CreateElementAtXY( -11, 0,0x01)
			gscriptable:CreateElementAtXY(  11, 0,0x01)							
			coroutine.yield()
			coroutine.yield()			
			
			gscriptable:CreateElementAtXY( 0,  11,0x00)
			gscriptable:CreateElementAtXY( 0, -11,0x00)							
			coroutine.yield()
			coroutine.yield()			
			
			
			
			gscriptable:CreateElementAtXYSpeed( -40,  40,270,30,0x01)
			gscriptable:CreateElementAtXYSpeed( -40, -40,90,30,0x01)
			coroutine.yield()
			coroutine.yield()			
			
			gscriptable:CreateElementAtXYSpeed( 40,  40,270,30,0x00)
			gscriptable:CreateElementAtXYSpeed( 40, -40,90, 30,0x00)				
			coroutine.yield()
			coroutine.yield()			
				
				
			gscriptable:CreateElementAtXYSpeed( -40,  40,270,30,0x00)
			gscriptable:CreateElementAtXYSpeed( -40, -40,90,30,0x00)
			coroutine.yield()
			coroutine.yield()			
			
			gscriptable:CreateElementAtXYSpeed( 40,  40,270,30,0x11)
			gscriptable:CreateElementAtXYSpeed( 40, -40,90, 30,0x11)				
			coroutine.yield()
			coroutine.yield()			
				
			
			for i = 0, 1, 1 do
		
			gscriptable:CreateElementAtXYSpeed( -40, -40,90,40,0x00)
			gscriptable:CreateElementAtXYSpeed(   0, -40,90,40,0x00)
			coroutine.yield()
			coroutine.yield()
			
			
			gscriptable:CreateElementAtXYSpeed( -20, 40,270,20,0x11)
			gscriptable:CreateElementAtXYSpeed(  20, 40,270,20,0x11)
			coroutine.yield()
			coroutine.yield()
			
			
			gscriptable:CreateElementAtXYSpeed(    0, -40,90,40,0x00)
			gscriptable:CreateElementAtXYSpeed(   40, -40,90,40,0x00)
			coroutine.yield()			
			coroutine.yield()
			
			
			gscriptable:CreateElementAtXYSpeed( -40,  40,270,40,0x00)
			gscriptable:CreateElementAtXYSpeed(   0,  40,270,40,0x00)
			coroutine.yield()			
			coroutine.yield()
			
			
			gscriptable:CreateElementAtXYSpeed( -20, -40,90,40,0x11)
			gscriptable:CreateElementAtXYSpeed(  20, -40,90,40,0x11)
			coroutine.yield()			
			coroutine.yield()
			
			
			gscriptable:CreateElementAtXYSpeed(    0, 40,270,40,0x00)
			gscriptable:CreateElementAtXYSpeed(   40, 40,270,40,0x00)
			coroutine.yield()			
			coroutine.yield()			
		
			end
		
				
			gscriptable:CreateElementAtXY( -10, 0,0x00)
			coroutine.yield()
			gscriptable:CreateElementAtXY(  10, 0,0x11)						
			coroutine.yield()
			gscriptable:CreateElementAtXY( 0,  10,0x00)
			coroutine.yield()
			gscriptable:CreateElementAtXY( 0, -10,0x11)
			coroutine.yield()
			
			
			gscriptable:CreateElementAtXY( -11, 0,0x00)
			coroutine.yield()
			gscriptable:CreateElementAtXY(  11, 0,0x11)						
			coroutine.yield()						
			gscriptable:CreateElementAtXY( 0,  11,0x11)
			coroutine.yield()
			gscriptable:CreateElementAtXY( 0, -11,0x00)			
			coroutine.yield()			
			
			
			gscriptable:CreateElementAtXY( -10, 0,0x11)
			coroutine.yield()
			gscriptable:CreateElementAtXY(  10, 0,0x00)			
			coroutine.yield()									
			gscriptable:CreateElementAtXY( 0,  10,0x11)
			coroutine.yield()
			gscriptable:CreateElementAtXY( 0, -10,0x00)
			coroutine.yield()
			
			
			gscriptable:CreateElementAtXY( -11, 0,0x11)
			coroutine.yield()
			gscriptable:CreateElementAtXY(  11, 0,0x00)			
			coroutine.yield()						
			gscriptable:CreateElementAtXY( 0,  11,0x00)
			coroutine.yield()
			gscriptable:CreateElementAtXY( 0, -11,0x11)
			coroutine.yield()
			coroutine.yield()
			coroutine.yield()
		
		
			
			
			
			
			gscriptable:CreateElementAtXY( -10, 0,0x00)
			gscriptable:CreateElementAtXY(  10, 0,0x00)			
			coroutine.yield()
			coroutine.yield()
			
			gscriptable:CreateElementAtXY( 0,  10,0x00)
			gscriptable:CreateElementAtXY( 0, -10,0x00)
			coroutine.yield()
			coroutine.yield()
			coroutine.yield()
			coroutine.yield()
			
			
			gscriptable:CreateElementAtXYSpeed( -40,  40,270,40,0x00)
			gscriptable:CreateElementAtXYSpeed( -40, -40,90,40,0x00)
			coroutine.yield()
			coroutine.yield()
			
			
			gscriptable:CreateElementAtXYSpeed( 40,  40,270,40,0x00)
			gscriptable:CreateElementAtXYSpeed( 40, -40,90, 40,0x00)				
			coroutine.yield()	
			coroutine.yield()
			
			
			gscriptable:CreateElementAtXY( -11, 0,0x01)
			gscriptable:CreateElementAtXY(  11, 0,0x01)							
			coroutine.yield()
			coroutine.yield()			
			
			gscriptable:CreateElementAtXY( 0,  11,0x00)
			gscriptable:CreateElementAtXY( 0, -11,0x00)							
			coroutine.yield()			
			coroutine.yield()			
			
			gscriptable:CreateElementAtXYSpeed( -40,  40,270,40,0x01)
			gscriptable:CreateElementAtXYSpeed( -40, -40,90,40,0x01)
			coroutine.yield()			
			coroutine.yield()			
			
			gscriptable:CreateElementAtXYSpeed( 40,  40,270,40,0x00)
			gscriptable:CreateElementAtXYSpeed( 40, -40,90, 40,0x00)				
			coroutine.yield()			
			coroutine.yield()			
				
				
			gscriptable:CreateElementAtXYSpeed( -40,  40,270,40,0x00)
			gscriptable:CreateElementAtXYSpeed( -40, -40,90,40,0x00)
			coroutine.yield()			
			coroutine.yield()			
			
			gscriptable:CreateElementAtXYSpeed( 40,  40,270,40,0x11)
			gscriptable:CreateElementAtXYSpeed( 40, -40,90, 40,0x11)				
			coroutine.yield()		
			coroutine.yield()			
			
			
			for i = 0, 1, 1 do
		
			gscriptable:CreateElementAtXYSpeed( -40, -40,90,40,0x00)
			gscriptable:CreateElementAtXYSpeed(   0, -40,90,40,0x00)
			coroutine.yield()
			coroutine.yield()
			
			
			gscriptable:CreateElementAtXYSpeed( -20, 40,270,20,0x11)
			gscriptable:CreateElementAtXYSpeed(  20, 40,270,20,0x11)
			coroutine.yield()
			coroutine.yield()
			
			
			gscriptable:CreateElementAtXYSpeed(    0, -40,90,40,0x02)
			gscriptable:CreateElementAtXYSpeed(   40, -40,90,40,0x02)
			coroutine.yield()			
			coroutine.yield()
			
			
			gscriptable:CreateElementAtXYSpeed( -40,  40,270,40,0x02)
			gscriptable:CreateElementAtXYSpeed(   0,  40,270,40,0x02)
			coroutine.yield()			
			coroutine.yield()
			
			
			gscriptable:CreateElementAtXYSpeed( -20, -40,90,40,0x11)
			gscriptable:CreateElementAtXYSpeed(  20, -40,90,40,0x11)
			coroutine.yield()			
			coroutine.yield()
			
			
			gscriptable:CreateElementAtXYSpeed(    0, 40,270,40,0x00)
			gscriptable:CreateElementAtXYSpeed(   40, 40,270,40,0x00)
			coroutine.yield()			
			coroutine.yield()			
		 
			end
			
			gscriptable:CreateElementAtXY( -10,   10,0x10)
			coroutine.yield()
			gscriptable:CreateElementAtXY(  10,   10,0x10)
			coroutine.yield()
			gscriptable:CreateElementAtXY(   0,  -10,0x10)
			
			coroutine.yield()
			coroutine.yield()	
			
				
			gscriptable:CreateElementAtXY( -10, 0,0x00)
			coroutine.yield()						
			gscriptable:CreateElementAtXY(  10, 0,0x11)			
			coroutine.yield()						
			gscriptable:CreateElementAtXY( 0,  10,0x00)
			coroutine.yield()						
			gscriptable:CreateElementAtXY( 0, -10,0x11)
			coroutine.yield()			
			
			
			gscriptable:CreateElementAtXY( -11, 0,0x00)
			coroutine.yield()						
			gscriptable:CreateElementAtXY(  11, 0,0x11)			
			coroutine.yield()					
			gscriptable:CreateElementAtXY( 0,  11,0x11)
			coroutine.yield()						
			gscriptable:CreateElementAtXY( 0, -11,0x00)
			coroutine.yield()			
			
			
			gscriptable:CreateElementAtXY( -10, 0,0x11)
			coroutine.yield()						
			gscriptable:CreateElementAtXY(  10, 0,0x00)			
			coroutine.yield()						
			gscriptable:CreateElementAtXY( 0,  10,0x11)
			coroutine.yield()						
			gscriptable:CreateElementAtXY( 0, -10,0x00)
			coroutine.yield()
			
			
			
			gscriptable:CreateElementAtXY( -11, 0,0x11)
			coroutine.yield()						
			gscriptable:CreateElementAtXY(  11, 0,0x00)			
			coroutine.yield()						
			gscriptable:CreateElementAtXY( 0,  11,0x00)
			coroutine.yield()						
			gscriptable:CreateElementAtXY( 0, -11,0x11)
			coroutine.yield()			
			coroutine.yield()			
			coroutine.yield()			
			
		
		
		
			gscriptable:CreateElementAtXY( -40, 30,0x00)
			gscriptable:CreateElementAtXY(   0, 30,0x00)			
			gscriptable:CreateElementAtXY(  40, 30,0x00)			
			coroutine.yield()
			coroutine.yield()
			gscriptable:CreateElementAtXY( -40,  -30,0x01)
			gscriptable:CreateElementAtXY(   0,  -30,0x01)
			gscriptable:CreateElementAtXY(  40,  -30,0x01)
			coroutine.yield()			
			coroutine.yield()
		
		
			gscriptable:CreateElementAtXY( -40,  10,0x00)
			coroutine.yield()
			gscriptable:CreateElementAtXY( -20,  10,0x01)
			coroutine.yield()
			gscriptable:CreateElementAtXY(  20,  10,0x00)
			coroutine.yield()
			gscriptable:CreateElementAtXY(  40,  10,0x01)
			coroutine.yield()
		
			gscriptable:CreateElementAtXY( -40,  -10,0x00)
			coroutine.yield()
			gscriptable:CreateElementAtXY( -20,  -10,0x01)
			coroutine.yield()
			coroutine.yield()
			gscriptable:CreateElementAtXY(  20,  -10,0x00)
			coroutine.yield()
			gscriptable:CreateElementAtXY(  40,  -10,0x01)
			coroutine.yield()
			coroutine.yield()
		
			
			gscriptable:CreateElementAtXY( -10,   10,0x10)
			coroutine.yield()
			gscriptable:CreateElementAtXY(  10,   10,0x10)
			coroutine.yield()
			gscriptable:CreateElementAtXY(   0,  -10,0x10)
			
			coroutine.yield()
			coroutine.yield()			
			
			
			
			gscriptable:CreateElementAtXYSpeed(   40, -40,135,40,0x00)
			gscriptable:CreateElementAtXYSpeed(  -40,  40,315,40,0x00)			
			
			gscriptable:CreateElementAtXYSpeed(   40,   40, 225,40,0x01)
			gscriptable:CreateElementAtXYSpeed(  -40,  -40,  45,40,0x01)			
			
			coroutine.yield()
			coroutine.yield()			
			
			gscriptable:CreateElementAtXY(   20,  -20,0x00)
			gscriptable:CreateElementAtXY(  -20,   20,0x00)			
			gscriptable:CreateElementAtXY(   20,   20,0x01)
			gscriptable:CreateElementAtXY(  -20,  -20,0x01)			
			gscriptable:Wait(6); 
					
			
			
			
			gscriptable:CreateElementAtXYSpeed(   40, -40,135,40,0x01)
			gscriptable:CreateElementAtXYSpeed(  -40,  40,315,40,0x02)						
			gscriptable:CreateElementAtXYSpeed(   40,   40, 225,40,0x01)
			gscriptable:CreateElementAtXYSpeed(  -40,  -40,  45,40,0x02)		
			coroutine.yield()
			coroutine.yield()			
						
			
			
			gscriptable:CreateElementAtXY(   20,  -20,0x01)
			gscriptable:CreateElementAtXY(  -20,   20,0x02)			
			gscriptable:CreateElementAtXY(   20,   20,0x01)
			gscriptable:CreateElementAtXY(  -20,  -20,0x02)			
			coroutine.yield()
			coroutine.yield()			
			
		
			
			-- FIN script
		
			
			gscriptable:OnScriptFinished()
			
			
         end)

function update(lscriptable)

  gscriptable = lscriptable
  
  coroutine.resume(co)  
  
end