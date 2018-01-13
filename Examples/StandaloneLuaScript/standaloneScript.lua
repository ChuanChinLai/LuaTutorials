--[[
	This is an example of a Lua script that stands alone,
	meaning that it is independent of any external program.

	The example StandaloneLuaScript project will load and execute it from C code,
	but it could also be run by $(OutputDir)lua.exe, the stand-alone Lua interpreter.
]]

-- C++ style comments are made like this
--[[ C style comments are made like this ]]
--[[ You can also nest C style comments by pairing the same number of equals signs within the double brackets:
	--[==[ Like this --[===[See? --[=====[Cool!]=====] ]===] ]==]
]]

-- You can display output using the print() function
print( "This is an example of the print() function" )

-- You don't need to declare a type for a variable;
-- here is an example of a string assigned to a variable:
myVariable = "This string is assigned to a variable"
print( myVariable )

-- You can concatenate two strings together using the ".." operator:
prefix = "This string"
suffix = "was constructed by concatenation"
print( prefix .. " " .. suffix )

-- There are "global" and "local" variables,
-- referring to their scope.
-- Just like in C/C++, you should prefer local variables.
-- If you don't specify otherwise a variable will be global,
-- and so you should usually prefix variable declarations with "local":
globalVariable = "This is a global variable"
local localVariable = "This is a local variable"

-- The basic variable types are "string", "number" (both floating point and integers), and "boolean":
local stringVariable = "This is a string"
local numberVariable = 123.456
local integerVariable = 789;
local booleanVariable = true
-- You can convert them to strings by using the tostring() function,
-- and you can find a variable's type by using the type() function:
print( "The variable stringVariable is a "
	.. type( stringVariable ) .. ": "
	.. tostring( stringVariable ) )
print( "The variable numberVariable is a "
	.. type( numberVariable ) .. " (" .. math.type( numberVariable ) .. "): "
	.. tostring( numberVariable ) )
	print( "The variable integerVariable is a "
	.. type( integerVariable ) .. " (" .. math.type( integerVariable ) .. "): "
	.. tostring( integerVariable ) )
print( "The variable booleanVariable is a "
	.. type( booleanVariable ) .. ": "
	.. tostring( booleanVariable ) )

-- There is also a type called "nil"
-- which is like NULL in C or C++:
local undefinedVariable = nil
print( "The variable undefinedVariable is a "
	.. type( undefinedVariable ) .. ": "
	.. tostring( undefinedVariable ) )

-- Any symbol that is not defined will be equal to nil:
print( "The value of an un-defined symbol is: "
	.. type( thisSymbolHasNeverBeenDefined ) )

-- A consequence of this is that you can define a local variable
-- without assigning it a value:
local anotherDefinedVariable
print( "The variable anotherDefinedVariable is a "
	.. type( anotherDefinedVariable ) .. ": "
	.. tostring( anotherDefinedVariable ) )
-- This can be useful as a form of forward declaration,
-- although this is more of a John-Paul style than idiomatic Lua;
-- you won't need to do this for our class,
-- but you may see me doing it in code that I provide.

-- If you are no longer using a variable you can set it to nil
-- (this signals to Lua that it can be garbage-collected):
stringVariable = nil
numberVariable = nil
integerVariable = nil
booleanVariable = nil

-- You can make a local scope by using do/end:
do
	local temporaryVariable = "This will go out of scope when the block ends"
	print( "The value of temporaryVariable inside the local scope is: "
		.. tostring( temporaryVariable ) )
end
print( "The value of temporaryVariable outside the local scope is: "
	.. tostring( temporaryVariable ) )
