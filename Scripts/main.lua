root = 'C:/Users/z1223/Desktop/1/verbo/'
model = 'build_char_437_mizuki'
w = 402
h = 506
x0 = 215
y0 = 106
x = 372
y = 840

ipc = require("hdtipc")

function init(w, h, x0, y0, x, y, atlas, skel)
	ipc.prepare(0, 4 * 7)
	ipc.writei32(0) -- use .skel file
	ipc.writei32(w)
	ipc.writei32(h)
	ipc.writei32(x0)
	ipc.writei32(y0)
	ipc.writei32(x)
	ipc.writei32(y)
	ipc.send()
	
	ipc.prepare(1, 100)
	ipc.writestr(atlas)
	ipc.send()
	
	ipc.prepare(2, 100)
	ipc.writestr(skel)
	ipc.send()
end

receivedTable = {
	[0] = function()
		init(math.ceil(w * 0.5), math.ceil(h * 0.5), x0, y0, x, y, root .. model .. '.atlas', root .. model .. '.skel')
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
	end
}

function received(type)
	s = receivedTable[type]
	if (s) then s() 
	else print('not support') end
end

ipc.reg("received")
