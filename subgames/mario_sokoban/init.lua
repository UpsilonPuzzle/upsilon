-- Nodes registration
upsilon.register_node("box", {
	texture = "box.jpg"
})
upsilon.register_node("box_ok", {
	texture = "box_ok.jpg"
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

-- Callbacks
upsilon.register_on_down(function()
	local mario_pos = upsilon.get_node_pos("mario")[0]
	local node = upsilon.get_node(mario_pos.x, mario_pos.y+1)
	if node == "wall" then
		return
	elseif node == "box" or node == "box_ok" then
		if upsilon.get_node(mario_pos.x, mario_pos.y+2) == "none" then
			upsilon.set_node(mario_pos.x, mario_pos.y+2, "box")
		elseif upsilon.get_node(mario_pos.x, mario_pos.y+2) == "goal" then
			upsilon.set_node(mario_pos.x, mario_pos.y+2, "box_ok")
			if #upsilon.get_node_pos("goal") == 0 then
				upsilon.next_map()
			end
		else
			return
		end
	end
	upsilon.move_node(mario_pos.x, mario_pos.y, 0, 1)
end)
upsilon.register_on_up(function()
	local mario_pos = upsilon.get_node_pos("mario")[0]
	local node = upsilon.get_node(mario_pos.x, mario_pos.y-1)
	if node == "wall" then
		return
	elseif node == "box" or node == "box_ok" then
		if upsilon.get_node(mario_pos.x, mario_pos.y-2) == "none" then
			upsilon.set_node(mario_pos.x, mario_pos.y-2, "box")
		elseif upsilon.get_node(mario_pos.x, mario_pos.y-2) == "goal" then
			upsilon.set_node(mario_pos.x, mario_pos.y-2, "box_ok")
			if #upsilon.get_node_pos("goal") == 0 then
				upsilon.next_map()
			end
		else
			return
		end
	end
	upsilon.move_node(mario_pos.x, mario_pos.y, 0, -1)
end)
upsilon.register_on_right(function()
	local mario_pos = upsilon.get_node_pos("mario")[0]
	local node = upsilon.get_node(mario_pos.x+1, mario_pos.y)
	if node == "wall" then
		return
	elseif node == "box" or node == "box_ok" then
		if upsilon.get_node(mario_pos.x+2, mario_pos.y) == "none" then
			upsilon.set_node(mario_pos.x+2, mario_pos.y, "box")
		elseif upsilon.get_node(mario_pos.x+2, mario_pos.y) == "goal" then
			upsilon.set_node(mario_pos.x+2, mario_pos.y, "box_ok")
			if #upsilon.get_node_pos("goal") == 0 then
				upsilon.next_map()
			end
		else
			return
		end
	end
	upsilon.move_node(mario_pos.x, mario_pos.y, 1, 0)
end)
upsilon.register_on_left(function()
	local mario_pos = upsilon.get_node_pos("mario")[0]
	local node = upsilon.get_node(mario_pos.x-1, mario_pos.y)
	if node == "wall" then
		return
	elseif node == "box" or node == "box_ok" then
		if upsilon.get_node(mario_pos.x-2, mario_pos.y) == "none" then
			upsilon.set_node(mario_pos.x-2, mario_pos.y, "box")
		elseif upsilon.get_node(mario_pos.x-2, mario_pos.y) == "goal" then
			upsilon.set_node(mario_pos.x-2, mario_pos.y, "box_ok")
			if #upsilon.get_node_pos("goal") == 0 then
				upsilon.next_map()
			end
		else
			return
		end
	end
	upsilon.move_node(mario_pos.x, mario_pos.y, -1, 0)
end)
