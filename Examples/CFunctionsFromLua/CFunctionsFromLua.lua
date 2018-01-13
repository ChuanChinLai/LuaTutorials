--[[
	This is an example of a Lua script that calls C/C++ functions
]]

-- All of the following functions have been registered in C,
-- which means that they can be called here just like regular Lua functions.
-- (Note that this script can _not_ be run on its own from the stand-alone interpreter
-- because it will try to call functions that don't exist.
-- It will only work properly when it is run after the functions have been added to the environment
-- like the CFunctionsFromLua example project does.)

ExampleFunction()

ExamplePrint( "Example string to print" )
ExamplePrint( 123.456 )

-- ExampleDouble
do
	local number_toDouble = 12.34
	local number_doubled = ExampleDouble( number_toDouble )
	print( "The result of doubling " .. number_toDouble
		.. " using ExampleDouble() is: " .. number_doubled )
end

-- ExampleAdd
do
	local numberToAdd1 = 1.0;
	local numberToAdd2 = 2.0;
	local sum = ExampleAdd( numberToAdd1, numberToAdd2 )
	print( "The result of adding " .. numberToAdd1 .. " + " .. numberToAdd2
		.. " using ExampleAdd() is: " .. sum )
end

-- ExampleStats
do
	local number1 = 1.2;
	local number2 = 3.4;
	local number3 = 5.6;
	local number4 = 7.8;
	-- Multiple return values are separated by commas:
	local sum, product, average = ExampleStats( number1, number2, number3, number4 )
	print( "Given "
		.. number1 .. ", " .. number2 .. ", " .. number3 .. ", and " .. number4
		.. ", ExampleStats() returns:\n"
		.. "\tsum = " .. sum .. ", product = " .. product .. ", average = " .. average )
end

-- ExampleError
do
	-- If the ExampleError() function were called normally, like this:
	--	* ExampleError()
	-- then an error would be thrown and this script wouldn't continue executing
	-- (you may want to try it to see what happens for yourself).

	-- However, just like there is a "protected" call function lua_pcall() in the C API
	-- there is a pcall() function in the Lua scripting language:
	local result, errorMessage = pcall( ExampleError )
	-- The first parameter to pcall() is the function that you want to call,
	-- and then you can include any further input parameters that you wish
	-- (in other words, the second parameter to pcall()
	-- will be the first parameter passed to the function).
	-- The first return value from pcall() is a boolean that will be true if there were no errors,
	-- and any remaining return values will be the standard return values if there were no errors
	-- or an error message if there were errors.
	-- (In other words, if there are no errors then the second return value from pcall()
	-- will be the first value that the given function returns.)
	if not result then
		print( errorMessage )
	end
end

-- ExampleErrorChecking
do
	-- This function expects a string as argument #1

	-- First it will be called correctly:
	do
		local result, errorMessage = pcall( ExampleErrorChecking, "This is the correct type" )
		if not result then
			print( errorMessage )
		end
	end
	-- Now it will be called incorrectly
	do
		local booleanArgument = false
		local result, errorMessage = pcall( ExampleErrorChecking, booleanArgument )
		if not result then
			print( errorMessage )
		end
	end
end
