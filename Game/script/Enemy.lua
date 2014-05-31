-- Constants:
local tileWidth = 16


-- Variables:
enemyId = 0
steps = 0
local collided = false
local success = false

function update( )
	if steps == tileWidth then
		resetSteps(enemyId)
		success,collided = checkCollision(enemyId)
		if success == true then
			if collided == true then
				collide(enemyId)
			end
		end
	end
end