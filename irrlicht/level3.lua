 gscriptable = nil
 

 co = coroutine.create(function ()
 
		-- INICIA script
		
		gscriptable:CreateElementAtXYSpeed(40,0,-180,20,0x00)
		gscriptable:CreateElementAtXYSpeed(-40,-1,0,20,0x00)
		coroutine.yield() 
		gscriptable:CreateElementAtXYSpeed(38,12,-162,20,0x01)
		gscriptable:CreateElementAtXYSpeed(-39,-13,18,20,0x01)
		coroutine.yield() 
		gscriptable:CreateElementAtXYSpeed(32,23,-144,20,0x02)
		gscriptable:CreateElementAtXYSpeed(-33,-24,36,20,0x02)
		coroutine.yield() 
		gscriptable:CreateElementAtXYSpeed(23,32,-126,20,0x00)
		gscriptable:CreateElementAtXYSpeed(-24,-33,54,20,0x00)
		coroutine.yield() 
		gscriptable:CreateElementAtXYSpeed(12,38,-108,20,0x01)
		gscriptable:CreateElementAtXYSpeed(-13,-39,72,20,0x01)
		coroutine.yield() 
		gscriptable:CreateElementAtXYSpeed(-1,39,-90,20,0x02)
		gscriptable:CreateElementAtXYSpeed(0,-40,90,20,0x02)
		coroutine.yield() 
		gscriptable:CreateElementAtXYSpeed(-13,38,-72,20,0x00)
		gscriptable:CreateElementAtXYSpeed(12,-39,108,20,0x00)
		coroutine.yield() 
		gscriptable:CreateElementAtXYSpeed(-24,32,-54,20,0x01)
		gscriptable:CreateElementAtXYSpeed(23,-33,126,20,0x01)
		coroutine.yield() 
		gscriptable:CreateElementAtXYSpeed(-33,23,-36,20,0x20)
		gscriptable:CreateElementAtXYSpeed(32,-24,144,20,0x20)
		coroutine.yield() 
		gscriptable:CreateElementAtXYSpeed(-39,12,-18,20,0x20)
		gscriptable:CreateElementAtXYSpeed(38,-13,162,20,0x20)
		coroutine.yield() 
		gscriptable:CreateElementAtXYSpeed(-40,-1,0,20,0x20)
		gscriptable:CreateElementAtXYSpeed(39,0,180,20,0x20)
		coroutine.yield() 
		gscriptable:CreateElementAtXYSpeed(-39,-13,18,20,0x02)
		gscriptable:CreateElementAtXYSpeed(38,12,198,20,0x02)
		coroutine.yield() 
		gscriptable:CreateElementAtXYSpeed(-33,-24,36,20,0x00)
		gscriptable:CreateElementAtXYSpeed(32,23,216,20,0x00)
		coroutine.yield() 
		gscriptable:CreateElementAtXYSpeed(-24,-33,54,20,0x01)
		gscriptable:CreateElementAtXYSpeed(23,32,234,20,0x01)
		coroutine.yield() 
		gscriptable:CreateElementAtXYSpeed(-13,-39,72,20,0x02)
		gscriptable:CreateElementAtXYSpeed(12,38,252,20,0x02)
		coroutine.yield() 
		gscriptable:CreateElementAtXYSpeed(0,-40,90,20,0x00)
		gscriptable:CreateElementAtXYSpeed(-1,39,270,20,0x00)
		coroutine.yield() 
		gscriptable:CreateElementAtXYSpeed(12,-39,108,20,0x01)
		gscriptable:CreateElementAtXYSpeed(-13,38,288,20,0x01)
		coroutine.yield() 
		gscriptable:CreateElementAtXYSpeed(23,-33,126,20,0x30)
		gscriptable:CreateElementAtXYSpeed(-24,32,306,20,0x30)
		coroutine.yield() 
		gscriptable:CreateElementAtXYSpeed(32,-24,144,20,0x30)
		gscriptable:CreateElementAtXYSpeed(-33,23,324,20,0x30)
		coroutine.yield() 
		gscriptable:CreateElementAtXYSpeed(38,-13,162,20,0x30)
		gscriptable:CreateElementAtXYSpeed(-39,12,342,20,0x30)
		coroutine.yield() 
		gscriptable:CreateElementAtXYSpeed(39,0,180,20,0x30)
		gscriptable:CreateElementAtXYSpeed(-40,-1,360,20,0x30)

		-- FIN script
		
		
		gscriptable:OnScriptFinished()
			
			
         end)

function update(lscriptable)

  gscriptable = lscriptable
  
  coroutine.resume(co)  
  
end