
testDrawLine  = function ()
	drawLine(0, 0, 300, 300)

	drawLine(100, 0, 300, 300)

	for i = 1, 10 do
	    drawLine(i*20, 200, i*10, 300)
	end
end

testDrawPoint = function ()
	drawPoint(150, 150)
end

testDrawRect = function ()
	drawRect(100, 100, 50, 50)
end

testFillRect = function ()
	fillRect(200, 200, 50, 50)
end

testSetColor = function ()
	setColor(255, 0, 0, 255)
end

testDrawLine()
testDrawPoint()
testDrawRect()
testSetColor()
testFillRect()
