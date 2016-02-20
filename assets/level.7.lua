 gscriptable = nil
 

 co = coroutine.create(function ()
 
		-- INICIA script
		
		 i = 1
			 
					
					
		 math.randomseed( os.time() )
			 
			
		level = 0
		
		 
		 for j = 0, 100, 1 do
		 
			if(j>0 and j%10==0) then
				level = level + 1
			end
			
			qty =10
			figures = 3
			timeout = 4000
			useDuo = 0
			useCross = 1
			
			if(level == 0) then
				qty = 6				
				figures = 2
				timeout = 6000
			elseif (level == 1) then
				qty = 8
				figures = 2
				timeout = 5000
			elseif (level == 2) then
				qty = 8				
				figures = 3
				timeout = 6000			
			elseif (level == 3) then
				qty = 10				
				figures = 3
				timeout = 7000								
			elseif (level == 4) then
				qty = 10			
				figures = 3
				timeout = 6000								
			elseif (level == 5) then
				qty = 10			
				figures = 3
				timeout = 5000											
			elseif (level == 6) then
				qty = 6
				figures = 3
				timeout = 12000								
				useDuo = 1
			elseif (level == 7) then
				qty = 6				
				figures = 3
				timeout = 12000								
				useDuo = 2
			else
				qty = 6				
				figures = 3
				timeout = 10000						
				useDuo = 3
			end
		 
			if(useCross == 1) then			
				gscriptable:CreateElementAtRandXY(0x40)				
			end
			
		 
			if (useDuo > 0) then
				
				for i = 1, useDuo, 1 do
				
					x= math.random(0,12)*10 - 60
					y= math.random(0,8)*10 - 40
					angle = math.random(0,8) * 45
		
					b =  math.random(1,figures) 
							 
					if (b==1) then
						gscriptable:CreateDuo(x,y, 5, angle, 0x00,0x01)												
					elseif (b==2) then
						gscriptable:CreateDuo(x,y, 5, angle, 0x00,0x02)												
					elseif (b==3) then
						gscriptable:CreateDuo(x,y, 5, angle, 0x01,0x02)												
					end 				
				
				end
			end
		 
			 for i = 1, qty, 1 do
			 
			  	b =  math.random(1,figures) 
							
							
				if (b==1) then
					gscriptable:CreateElementAtRandXY(0x00)
				elseif (b==2) then
					gscriptable:CreateElementAtRandXY(0x01)
				elseif (b==3) then
					gscriptable:CreateElementAtRandXY(0x02)												
				end 
			
				
			 end
			 
			 
			gscriptable:WaitForClear(-1)
			coroutine.yield()
		 
	    end
		
		-- FIN script
		
		
		gscriptable:OnScriptFinished()
			
			
         end)

function update(lscriptable)

  gscriptable = lscriptable
  
  coroutine.resume(co)  
  
end