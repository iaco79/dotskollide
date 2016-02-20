 gscriptable = nil
 

 co = coroutine.create(function ()
 
			-- INICIA script
			
			 i = 1
			 
					
					
			 math.randomseed( os.time() )
			 
			x= math.random(0,6)*10 - 30
			y= math.random(0,6)*10 - 30
			angle = math.random(0,8) * 45
		
		    gscriptable:CreateElementDistributed(1)				
			gscriptable:WaitForCrash(-1)
			gscriptable:WaitForClear(-1)
		    coroutine.yield()
		
			for i = 0, 400, 1 do
			 
			   x= math.random(0,6)*10 - 30
			   y= math.random(0,6)*10 - 30
		   	   angle = math.random(0,8) * 45		
		
				b = math.random(0,9)
			  
			  --gscriptable:CreateElementDistributed(1)	
			  
			   if (b>=7 and b<=9) then
				 gscriptable:CreateTri(x,y,7,angle, 0x0f,0x0f,0x0f)					
			   elseif (b>=5 and b<=6) then
				 gscriptable:CreateTri2(x,y,12,angle, 0x0f,0x0f,0x0f)									
			   elseif (b>=1 and b<=4) then
				 gscriptable:CreateElementDistributed(2)	
			   else
			    gscriptable:CreateElementDistributed(1)	
			   end
			   
			--   if(i>10) then
				--gscriptable:CreateElementAtRandXY(0x40)	
			   --end
			  
			   
				gscriptable:WaitForCrash(-1)
				gscriptable:WaitForClear(-1)
				
				coroutine.yield()
								
			 end
			 
			-- FIN Script
			
		
			gscriptable:OnScriptFinished()
			
			 
         end)

function update(lscriptable)

  gscriptable = lscriptable
  
  coroutine.resume(co)  
  
end