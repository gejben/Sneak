-- Constants:
local tileWidth = 16

local states = {
	PATROLLING 	= 0,
	CHASING 	= 1,
	RETURNING	= 2
}


-- Variables:
enemyId = 0
local pathFound = false
local state = 0
local collided = false
local success = false

local file
file = io.open("debug.txt", "w") -- clear file every time program starts
file:write()
file:close()

function update( )
	file = io.open("debug.txt", "a")
	if state == 0 then
		file:write("state - Patrolling\n")
		if checkForPlayer(enemyId) == true then
			state = 1
		end
	elseif state == 1 then
		file:write("state - Chasing\n")
		if checkForPlayer(enemyId) == false then
			state = 2
		else 
			chase(enemyId)
		end
	elseif state == 2 then
		file:write("state - Returning\n")
		if pathFound == false then
			file:write("search path\n")
			searchStartpos(enemyId)
			pathFound = true
			file:write("path found\n")
		end
		nextTile(enemyId)
		file:write("go to next tile\n")
		if returnedToStart(enemyId) then
			pathFound = false
			state = 0
		end
	end
	file:write("return:" .. state .. "\n")
	file:close()
	return state
end