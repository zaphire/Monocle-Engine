io.write "Something\n"

Point = {}

Point.new = function(x, y)
    local self = {}
    x = x or 0
    x = y or 0
    
    self.getx = function() return x end
    self.gety = function() return y end
    self.write = function() io.write(string.format("(%d, %d)\n", x, y)) end
    
    return self
end

p = Point.new(2,5)
p.write()

function HelloWorld()
	io.write "Hello, World!\n"
	local test = true
	if test == true then
	    io.write "it's true!\n"
    end
end
