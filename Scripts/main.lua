root = hdt.basedir()
package.path = root .. '\\Scripts\\?.lua'
json = require('json')

read_model_config_file = io.open(root .. '\\Scripts\\model.json', 'r')
model_config = json.decode(read_model_config_file:read())
read_model_config_file:close()

config_path = root .. '\\Scripts\\config.json'
read_config_file = io.open(config_path, 'r')
config = json.decode(read_config_file:read())
read_config_file:close()
x = config.x
y = config.y
scale = config.scale

ipc = require("hdtipc")
hdt = require("hdt")

function init(w1, h1, x0, y0, x, y, scale, atlas, skel)
	w = math.ceil(scale * w1)
	h = math.ceil(scale * h1)
	x0 = x0 * scale
	y0 = h - (h1 - y0) * scale

	ipc.prepare(0, 4 * 8)
	
	ipc.writei32(0) -- use .skel file
	
	ipc.writei32(w)
	ipc.writei32(h)
	ipc.writefloat(x0)
	ipc.writefloat(y0)
	
	ipc.writei32(x)
	ipc.writei32(y)
	ipc.writefloat(scale)
	
	ipc.send()
	
	ipc.prepare(1, atlas:len() + 10)
	ipc.writestr(atlas)
	ipc.send()
	
	ipc.prepare(2, skel:len() + 10)
	ipc.writestr(skel)
	ipc.send()
end

function writeconfig(x, y)
	config.x = x
	config.y = y
	local f
	f = io.open(config_path, 'w')
	f:write(json.encode(config))
	f:close()
end

receivedTable = {
	[0] = function()
		init(model_config.w, model_config.h, model_config.x0, model_config.y0, x, y, scale, root .. '\\' .. model_config.model .. '.atlas', root .. '\\' .. model_config.model .. '.skel')
	end,
	[2] = function()
		print('Drag start.')
	end,
	[3] = function()
		local stx
		local sty
		local x
		local y
		stx, x = ipc.readi32()
		sty, y = ipc.readi32()
		print('Drag end. x = ', x, ', y = ', y)
		writeconfig(x, y)
	end
}

function received(type)
	s = receivedTable[type]
	if (s) then s() 
	else print('not support') end
end

ipc.reg("received")
