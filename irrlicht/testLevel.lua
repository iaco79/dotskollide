 gscriptable = nil
 

 
function tutorial(step)
  
  if (gscriptable:IsRunningTutorial()>0) then  
			gscriptable:SetLevelValues(3,15,  1, 0, 20, 1,3)
  
			gscriptable:SetTutorialText("Connect 2 dots.",1)			
			gscriptable:CreateElementAtXY( 20,0,0x00)
			gscriptable:CreateElementAtXY(-20,0,0x00)						
			gscriptable:WaitForTap(-2)			
			coroutine.yield()					
  end
  if (gscriptable:IsRunningTutorial()>0) then    
			gscriptable:WaitForCrash(-1)
			coroutine.yield()			
  end  
  if (gscriptable:IsRunningTutorial()>0) then      
			gscriptable:SetTutorialText("2 or 3 dots of the same color explode creating energy.",1)			
			gscriptable:WaitForTap(-2)
			coroutine.yield()						
  end  
  if (gscriptable:IsRunningTutorial()>0) then 
			gscriptable:SetTutorialText("Fill the energy bars and you will advance to the next level.",1)			
			gscriptable:WaitForTap(-2)
			coroutine.yield()						
  end
  if (gscriptable:IsRunningTutorial()>0) then 	
			gscriptable:SetTutorialText("Connect 2 dots.",1)		
			gscriptable:CreateElementAtXY( 20,0,0x00)
			gscriptable:CreateElementAtXY(-20,0,0x01)						
			gscriptable:WaitForTap(-2)
			coroutine.yield()			
  end
  if (gscriptable:IsRunningTutorial()>0) then 	
			gscriptable:WaitForJoin(-1)
			coroutine.yield()
  end
  if (gscriptable:IsRunningTutorial()>0) then 	
			gscriptable:SetTutorialText("Dots of different color join into a mole.",1)			
			gscriptable:WaitForTap(-2)
			coroutine.yield()
  end
  if (gscriptable:IsRunningTutorial()>0) then 	  
			gscriptable:CreateDuo(0,  20, 5, 0, 0x000,0x001)
			gscriptable:SetTutorialText("Connect 2 moles.",1)					
			gscriptable:WaitForTap(-2)
			coroutine.yield()					
  end			
  if (gscriptable:IsRunningTutorial()>0) then 	  
			gscriptable:WaitForCrash(-1)
			coroutine.yield()
  end
  if (gscriptable:IsRunningTutorial()>0) then 	  
			gscriptable:SetTutorialText("Moles that are color equivalent react.",1)			
			gscriptable:WaitForTap(-2)
			coroutine.yield()				
  end
  if (gscriptable:IsRunningTutorial()>0) then 	  	
			gscriptable:CreateElementAtXY( 20,-10,0x00)
			gscriptable:CreateElementAtXY(-20,-10,0x01)	
			gscriptable:CreateElementAtXY(  0, 10,0x02)				
			gscriptable:SetTutorialText("Connect 3 dots.",1)					
			gscriptable:WaitForTap(-2)
			coroutine.yield()
  end  
  if (gscriptable:IsRunningTutorial()>0) then 	  	
			while (gscriptable:GetMoleCount() ~= 1) do			
				
				if (gscriptable:IsRunningTutorial()>0) then 	  					
					gscriptable:WaitForJoin(2000)
					coroutine.yield()
				else
					break
				end
				
			end
  end  
  if (gscriptable:IsRunningTutorial()>0) then 	  	
			gscriptable:SetTutorialText("You can connect up to 3 dots creating a mole-3",1)			
			gscriptable:WaitForTap(-2)
			coroutine.yield()				
  end  
  if (gscriptable:IsRunningTutorial()>0) then 	  	
			gscriptable:CreateDuo(0,  0, 5, 0, 0x001,0x002)
			gscriptable:SetTutorialText("Connect the mole-2 with the mole-3.",1)					
			gscriptable:WaitForTap(-2)
			coroutine.yield()
  end
  if (gscriptable:IsRunningTutorial()>0) then 	  	
			gscriptable:WaitForCrash(-1)
			coroutine.yield()
  end
  if (gscriptable:IsRunningTutorial()>0) then 	  	
			gscriptable:SetTutorialText("You can partially react moles if they are color equivalent.",1)
			gscriptable:WaitForTap(-2)
			coroutine.yield()				
  end  
  if (gscriptable:IsRunningTutorial()>0) then 	  		
			gscriptable:SetTutorialText("Create a mole-2.",1)		
			gscriptable:CreateElementAtXY( 0,0,0x01)			
			gscriptable:WaitForTap(-2)
			coroutine.yield()						
  end			
  if (gscriptable:IsRunningTutorial()>0) then 	  		
			gscriptable:WaitForJoin(-1)
			coroutine.yield()			
  end
  if (gscriptable:IsRunningTutorial()>0) then 	  		
	
			gscriptable:CreateDuo(0,  0, 5, 0, 0x001,0x002)			
			gscriptable:SetTutorialText("Try to react the 2 moles.",1)					
			gscriptable:WaitForTap(-2)
			coroutine.yield()
  end		
  if (gscriptable:IsRunningTutorial()>0) then 	  		
			gscriptable:WaitForCrash(3000)
			coroutine.yield()
  end
  if (gscriptable:IsRunningTutorial()>0) then 	  		
			gscriptable:SetTutorialText("Moles that are not color equivalent won't react.",1)			
			gscriptable:WaitForTap(-2)
			coroutine.yield()							
  end	
  if (gscriptable:IsRunningTutorial()>0) then 	  			
			gscriptable:CreateDuo(0,  0, 5, 0, 0x000,0x002)
			gscriptable:SetTutorialText("Try to react the 3 moles.",1)											
			gscriptable:WaitForTap(-2)
			coroutine.yield()
  end
  if (gscriptable:IsRunningTutorial()>0) then 	  		
			gscriptable:WaitForCrash(-1)
			coroutine.yield()
  end		
  if (gscriptable:IsRunningTutorial()>0) then 	  		
			gscriptable:SetTutorialText("Complementary moles will react.",1)			
			gscriptable:WaitForTap(-2)
			coroutine.yield()			
  end
  if (gscriptable:IsRunningTutorial()>0) then 	  		
			gscriptable:SetTutorialText("This is the end of the tutorial. Good  Luck!",1)			
			gscriptable:WaitForTap(-2)
			coroutine.yield()
			
  end
	
  if (gscriptable:IsRunningTutorial()>0) then									
		gscriptable:OnTutorialFinished()
		coroutine.yield()
  end
					
end 
 
 co = coroutine.create(function ()
 
			-- INICIA script
			
			 
			level = 1	
			
			
			isTutorial = 0
			
			
			tutorialStep = 0
            lastlevel = 0
			waitTime  =2000			
		    math.randomseed(os.time() )			 			
			
			gscriptable:SetLevelValues(3,10,  1, 0, 20, 1,1)
			coroutine.yield()
			
			while 1==1 do
			 
			
				isTutorial = gscriptable:IsRunningTutorial()
				
				if (isTutorial==1 or isTutorial==2)	then
					lastlevel=0					
					if (isTutorial==1) then						
						tutorialStep = 0
					end					
					if (gscriptable:IsRunningTutorial()>0) then
					   tutorial(tutorialStep)	
					end
				else				
					if (level == 200) then
							gscriptable:SetTutorialText("Game Over.",1)
							gscriptable:WaitForTap(-2)
							coroutine.yield()
							gscriptable:OnTutorialFinished()
							coroutine.yield()		
					else 
						level = gscriptable:CalculateLevel()
					
				
						if (lastlevel~=level) then
						
							lastlevel = level
							
							if (level==1) then
								gscriptable:SetLevelValues(3,10,  1, 1, 30, 1,1)
								gscriptable:SetTutorialText("Don't let dots go away! Collide the dots.",1)
								gscriptable:WaitForTap(-1)
								coroutine.yield()
								
							elseif (level==2) then						
								gscriptable:SetLevelValues(3,12,  1, 4, 30, 1,2)
							elseif (level==3) then											
								gscriptable:SetLevelValues(3,15,  1, 5, 30, 1,3)
							elseif (level==4) then											
								gscriptable:SetLevelValues(3,15,  1, 6, 30, 1,4)
							elseif (level==5) then											
								gscriptable:SetLevelValues(3,15,  1, 7, 30, 1,5)
								
							elseif (level==6) then
								gscriptable:SetLevelValues(3,10, 1, 4, 20, 2,2)							
								gscriptable:SetTutorialText("Don't let dots go away! Collide at least Mole-2.",1)
								gscriptable:WaitForTap(-1)
								coroutine.yield()	
								
							elseif (level==7) then
								gscriptable:SetLevelValues(3,12, 1, 5, 20, 2,3)						
							elseif (level==8) then						
								gscriptable:SetLevelValues(3,15, 1, 6, 30, 2,4)						
							elseif (level==9) then						
								gscriptable:SetLevelValues(3,15, 1, 7, 30, 2,5)
								
							elseif (level==10) then						
								gscriptable:SetLevelValues(3,12, 1, 4, 20, 3,3)
								gscriptable:SetTutorialText("Don't let dots go away! Collide at least Mole-3",1)
								gscriptable:WaitForTap(-1)
								coroutine.yield()
								
							elseif (level==11) then						
								gscriptable:SetLevelValues(3,15, 1, 5, 20, 3,4)
							elseif (level==12) then						
								gscriptable:SetLevelValues(3,15, 1, 6, 30, 3,5)
							else
								gscriptable:SetLevelValues(3,15, 1, 7, 30, 3,5)
							end
							
						
						end		
				
						--launch random elements				
					   b = math.random(0,9)
					   if (b>=8 and b<=9) then
						 gscriptable:CreateElementDistributed(4)	
					   elseif (b>=6 and b<=7) then 
						 gscriptable:CreateElementDistributed(3)	
					   elseif (b>=3 and b<=5) then
						 gscriptable:CreateElementDistributed(2)	
					   else
						gscriptable:CreateElementDistributed(1)	
					   end
					
						gscriptable:WaitForCrash(waitTime)				
						coroutine.yield()
					end
				end
			 end
			 
			-- FIN Script
			
		
			gscriptable:OnScriptFinished()
			
			 
         end)

function update(lscriptable)

  gscriptable = lscriptable
  
  coroutine.resume(co)  
  
end