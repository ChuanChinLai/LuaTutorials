--[[
	This is an example of both a dictionary and an array
	in the same table, and how to read those values in C/C++
]]

return
{
	-- String keys:
	name = "Fred",
	age = 42,

	-- Implicit integer keys:
	"first", "second", "third",
}

--[[
	As a review, the table above uses convenience syntax.
	An identical table could be made doing the following:
	local explicitTable = {}
	explicitTable["name"] = "Fred"
	explicitTable["age"] = 42

	explicitTable[1] = "first"
	explicitTable[2] = "second"
	explicitTable[3] = "third"
]]
