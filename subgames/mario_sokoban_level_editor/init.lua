-- Nodes registration
upsilon.register_node("box", {
	texture = "box.jpg"
})
upsilon.register_node("goal", {
	texture = "goal.png"
})
upsilon.register_node("mario", {
	texture = "mario.gif"
})
upsilon.register_node("wall", {
	texture = "wall.jpg"
})

-- Write map file at exit
upsilon.register_at_exit(function()
	if #upsilon.get_node_pos("mario") == 0 then
		print("Mario sokoban level editor: error: you have to place mario somewhere (using mouse right-click).")
		return
	end
	
	local level = 1
	local f = io.open("subgames/mario_sokoban/map/map1.lua", "r")
	while f ~= nil do
		f:close()
		level = level+1
		f = io.open("subgames/mario_sokoban/map/map"..tostring(level)..".lua", "r")
	end

	f = io.open("subgames/mario_sokoban/map/map"..tostring(level)..".lua", "w")
	f:write("map.nodes = {\n")
	for y = 0, 9 do
		f:write("{")
		for x = 0, 9 do
			f:write("\"")
			f:write(upsilon.get_node(x, y))
			f:write("\", ")
		end
		f:write("},\n")
	end
	f:write("}")
	f:close()
end)

-- Callbacks
upsilon.register_on_left_click(function(x, y)
	local nodename = upsilon.get_node(x, y)
	if nodename == "none" then
		upsilon.set_node(x, y, "wall")
	elseif nodename == "wall" then
		upsilon.set_node(x, y, "box")
	elseif nodename == "box" then
		upsilon.set_node(x, y, "goal")
	elseif nodename == "goal" then
		upsilon.remove_node(x, y)
	end
end)

upsilon.register_on_right_click(function(x, y)
	local nodename = upsilon.get_node(x, y)
	if nodename == "mario" then
		upsilon.remove_node(x, y)
		return
	end
	
	local mario_pos = upsilon.get_node_pos("mario")
	if #mario_pos > 0 then
		upsilon.remove_node(mario_pos[1].x, mario_pos[1].y)
	end
	upsilon.set_node(x, y, "mario")
end)
