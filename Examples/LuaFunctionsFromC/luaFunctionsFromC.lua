--[[
	This is an example of a Lua script that defines a function
	that a C/C++ program can call
]]

-- This file will be loaded and executed just like standaloneScript.lua was
print( "This statement will be displayed when this file is loaded and executed" )

-- However, in addition to any Lua statements that will be executed immediately
-- this file will define a functions that can be called later

-- The first example has no input parameters and doesn't return any values:
function ExampleFunction()
	print( "This statement will only be displayed when this function is called" )
end

-- This function has a single input parameter:
function ExamplePrint( i_value )
	print( "The input parameter to ExamplePrint() is: " .. tostring( i_value ) )
end

-- This function has a single input parameter and single return value:
function ExampleDouble( i_value )
	local o_value = i_value + i_value
	return o_value
end

-- This function has multiple input parameters and a single return value:
function ExampleAdd( i_value1, i_value2 )
	return i_value1 + i_value2
end

-- This function has multiple input parameters and multiple return values:
function ExampleStats( i_value1, i_value2, i_value3, i_value4 )
	local o_sum = i_value1 + i_value2 + i_value3 + i_value4
	local o_product = i_value1 * i_value2 * i_value3 * i_value4
	local o_average = o_sum / 4.0

	return o_sum, o_product, o_average
end

-- This function has an error in it
-- (refer to the C/C++ code for how to handle this):
function ExampleError()
	-- Trying to add two undeclared values is undefined
	-- (or, more precisely, trying to add two nil values is an error)
	local result = badVariable1 + badVariable2
end

-- This function will do error checking to validate
-- the input parameters from the caller
function ExampleErrorChecking( i_argument )
	-- This hypothetical function expects a string for an argument
	if type( i_argument ) == "string" then
		print( "A string was correctly passed as argument #1 to ExampleErrorChecking()" )
	else
		-- You can throw an error in Lua using the error() function:
		error( "A " .. type( i_argument ) ..
			" was incorrectly passed as argument #1 to ExampleErrorChecking() " ..
			"(instead of a string)" )
	end
end
