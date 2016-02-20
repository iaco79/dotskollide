 gscriptable = nil
 

 co = coroutine.create(function ()
 
 
			-- INICIA script
			
			--Create 5 duos
			gscriptable:CreateDuo(0, 40, 15, 0, 0x001,0x100);
			gscriptable:CreateDuo(0, 20, 15, 0, 0x100,0x002);
			gscriptable:CreateDuo(0,  0, 15, 0, 0x000,0x101);
			gscriptable:CreateDuo(0,-20, 15, 0, 0x102,0x000);
			gscriptable:CreateDuo(0,-40, 15, 0, 0x000,0x100);
			
			--wait for clear  /  no timeout
			gscriptable:WaitForClear(-1);
			coroutine.yield() 
			
			--create tetra and wait 1 second
		    gscriptable:CreateTetra(0, 0, 15, 90, 0x10,0x11,0x12,0x120);
			gscriptable:Wait(1000);
			coroutine.yield()  

			--create 4 elements and wait 2 seconds
			gscriptable:CreateElementAtXY(  0, 40, 0x00)			
			gscriptable:CreateElementAtXY(-35,-35, 0x01)			
			gscriptable:CreateElementAtXY( 35,-35, 0x02)			 
			gscriptable:Wait(2000); 
			coroutine.yield()  
			
			 
			 --create 2 triangles and wait for  clear / 10 seconds time out
			gscriptable:CreateElementAtXY(-30,  0, 0x20)			
			gscriptable:CreateElementAtXY( 30,  0, 0x20)			
			gscriptable:WaitForClear(10000);
			coroutine.yield() 

			--create 2 Tris 
			gscriptable:CreateTri(0, 0, 15,   0, 0x00,0x01,0x02);
			gscriptable:CreateTri(0, 0, 15, 180, 0x00,0x02,0x01);
			coroutine.yield()  
			
			-- FIN Script
			
		
			gscriptable:OnScriptFinished()
			
			 
         end)

function update(lscriptable)

  gscriptable = lscriptable
  
  coroutine.resume(co)  
  
end