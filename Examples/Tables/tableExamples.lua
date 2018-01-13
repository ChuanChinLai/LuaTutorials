--[[
	This file contains examples of using tables in Lua scripts
]]

-- Introduction
--=============

-- Tables are Lua's data structures.
-- They are created using curly braces:
local myTable = {}

-- Tables work using key/value pairs.
-- _Anything_ can be stored in a table,
-- and _anything_ can be used as the key to store it.

-- The basic syntax of storing a value is:
--	t[k] = v
-- where t is a table, k is a key, and v is a value.
-- For example:
do
	myTable[123] = "This is a string"
	myTable["stringKey"] = 456
	myTable[789.123] = true
	myTable[false] = "This is another string"
end

-- After the four statements above
-- there are four values stored in myTable.
-- Note that any value type can be used as a key
-- (the examples show numbers, a string, and a boolean),
-- and any value type can be stored in a table
-- (and, as the examples show, these value types can be mixed and matched.)

-- The basic syntax of reading a value is:
--	t[k]
-- where t is a table and k is a key.
-- For example:
do
	local value1 = myTable[123]
	local value2 = myTable["stringKey"]

	print( "What was at [123]? " .. tostring( value1 ) )
	print( "What was at [\"stringKey\"]? " .. tostring( value2 ) )
end

-- If you try to get a value from a table
-- using a key that isn't stored in a table
-- you will get a nil
-- (which, remember, is like NULL in C/C++):
do
	local nonExistentValue = myTable[3.14]
	
	print( "What was at [3.14]? " .. tostring( nonExistentValue ) )
end

-- Similarly, you can remove a value from a table
-- by assigning nil:
do
	-- Originally no value exists at the example key:
	local exampleKey = "test"
	local value = myTable[exampleKey]
	print( "Originally, what is at the example key? " .. tostring( value ) )

	-- A value can be assigned:
	myTable[exampleKey] = "STOREDVALUE"
	value = myTable[exampleKey]
	print( "After assigning something, what is at the example key? " .. tostring( value ) )

	-- That value can be removed like this:
	myTable[exampleKey] = nil
	value = myTable[exampleKey]
	print( "After assigning nil, nothing is at the example key: " .. tostring( value ) )
end

-- Lua provides the general iterator function pairs() for iterating through
-- every key/value pair in a table:
for key, value in pairs( myTable ) do
	print( "\"key\" is: " .. tostring( key ) )
	print( "\t\"value\" is: " .. tostring( value ) )
end
-- Note that the order that elements are returned using pairs() is non-deterministic!
-- It depends on the internal workings and hashing of Lua,
-- and no order should ever be assumed.
-- Forgetting this in later assignments
-- leads to one of the most common bugs that students encounter in this class,
-- so try to make a mental note: "General table iteration in Lua is non-deterministic" :)

-- "Arrays" and "Dictionaries"
--============================

-- Even though any value type can be used as a key
-- by far the most commonly used keys are integers and strings.
-- Each of these has specialized syntax
-- to make them easier to read and easier to work with.

-- Arrays
---------

-- A table that has integers as its keys functions like an array:
-- It is an ordered list of values.
-- You can create an array like this:
local array = { "firstValue", "secondValue", "thirdValue" }
-- Note the syntax: It is a comma-separated list.
-- The single line above creates the _exact_ same table as doing this:
do
	local array = {}
	array[1] = "firstValue"
	array[2] = "secondValue"
	array[3] = "thirdValue"
end
-- Note that arrays in Lua start with 1 and not 0.
-- THIS IS IMPORTANT, and is often confusing after being used to C/C++.
-- Any number can be used as a key, and so it is valid to do this:
array[0] = "someValue"
-- The table now holds four values,
-- but the _array_ is still considered to only have three values,
-- at keys [1], [2], and [3].
-- You can get the length of an array using the # operator:
do
	local arrayLength = #array
	print( "How many items are in array? " .. tostring( arrayLength ) )
end
-- If you add another element at the end the array will get bigger:
do
	array[4] = "fourthValue"
	print( "How many items are in array now? " .. tostring( #array ) )
end
-- Note that not only does a valid array always start at 1,
-- but it always must have sequential integer keys.
-- We could skip [5] and add a value at [6],
-- but then the table is no longer a valid array:
do
	array[6] = "What will this do?"
	print( "How many items are in array now? " .. tostring( #array ) )
	-- Doing this is risky, and the result of #array can't be relied upon.
	-- When I was writing/testing this, the above statement would print 6,
	-- but if I did _this_:
	array[9] = "Now what?"
	print( "How many items are in array now? " .. tostring( #array ) )
	-- then #array was still 6, rather than 9.

	-- In other words, you might _sometimes_ get results that seem to make sense,
	-- but you can other times get values that seem wrong.
	-- The only way to get consistent results is to
	-- always have continguous integer values starting at 1 if you want array behavior.
end

-- A better way to add/append values to an array is using the following idiom:
do
	array[#array + 1] = "anotherValue"
	array[#array + 1] = "andAnotherValue"
	array[#array + 1] = "andSoOn"
end

-- Lua provides a special iterator function for iterating through arrays:
for key, value in ipairs( array ) do
	print( "\"key\" should be an integer (starting at 1): " .. tostring( key ) )
	print( "\t\"value\" should be whatever we stored previously: " .. tostring( value ) )
end
-- Unlike the general pairs() iterator function,
-- ipairs() is _guaranteed_ to return the values in an iterator in order.

-- It took me awhile to remember the difference between pairs() and ipairs() without looking it up.
-- The trick that I use now is to think of "ipairs"
-- as the classic C/C++ for loop using the temporary variable "i":
for i, value in ipairs( array ) do
	-- The "i" in this "ipairs" iterating loop is just a counter
end

-- Finally, note that an array is only special because it uses integer keys.
-- Even though my example used all string values,
-- the values can actually be anything (like any other table):
local mixedArray = { 123, 234, "a string", true, 345, "another string", 456 }

-- Dictionaries
---------------

-- A table that has strings for keys functions like a dictionary
-- where you can look up a value by its associated "name".
-- You can create dictionaries like this:
local dictionary = { red = 12, green = 23, blue = 34 }
-- Notice that there is a comma-separated list of _named_ values
-- (and note that the names are not in quotes, even though they're strings).
-- The line above is the same as doing the following:
do
	local dictionary = {}
	dictionary["red"] = 12
	dictionary["green"] = 23
	dictionary["blue"] = 34
end

-- The values can be any type:
local anotherDictionary = { name = "John-Paul", age = 17, isLiar = false }

-- Not only is their special syntax for creating dictionaries,
-- but there is special syntax for working with them using the . operator:
do
	local redValue = dictionary.red
	dictionary.alpha = 45
end

-- This means that working with a "dictionary" table
-- is like working with a C/C++ struct:
-- You have some kind of data structure with named elements.
-- This syntax is _very_ convenient and readable,
-- but it is important to remember that it's just a fancy
-- way of using strings as keys:
do
	-- General way:
	anotherDictionary["isTeacher"] = true
	-- Specialized but equivalent way:
	anotherDictionary.isTeacher = true
end

-- Finally, note that you can mix arrays and dictionaries together:
local mixedArrayAndDictionary =
{
	-- Array part, ordered list:
	"value1", "value2", "value3", "value4",
	-- Dictionary part, unordered list with string keys:
	name = "my table", importance = 10000, isCool = true
}

-- If you use ipairs() on a mixed table you will _only_ get the ordered part:
print( "Using ipairs() on a mixed table:" )
for i, orderedValue in ipairs( mixedArrayAndDictionary ) do
	print( "\tOrdered value #" .. tostring( i ) .. ": " .. tostring( orderedValue ) )
end

-- If you use pairs() on a mixed table you will get every key/value pair,
-- _including_ the integer array key/values:
print( "Using pairs() on a mixed table:" )
for key, unorderedValue in pairs( mixedArrayAndDictionary ) do
	print( "\tUnordered " .. type( key ) .. " key: " .. tostring( key ) )
	print( "\t\tValue = " .. tostring( unorderedValue ) )
end

-- As a reminder, the iteration order of pairs() is nondeterministic!
-- FORGETTING THIS IS ONE OF THE MOST COMMON MISTAKES WITH LUA THAT PEOPLE MAKE IN THIS CLASS!
-- If you store a vector like { x = 234, y = 123, z = 345 }
-- and iterate through the table using pairs() you are _not_
-- guaranteed to get them in the order x, y, z, even though you declared them that way!

-- Tables Within Tables
--=====================

-- Remember that _any_ value type can be stored in a table.
-- This means that tables can be stored in tables:
do
	local outerTable = {}
	local innerTable = {}
	outerTable.myTable = innerTable
end
-- The above code could also be written like this:
do
	local outerTable = { myTable = {} }
end
-- Or, since whitespace doesn't matter:
do
	local outerTable1 = {
		myTable = {}
	}
	local outerTable2 =
	{
		myTable = {}
	}
	local outerTable3 =
	{
		myTable =
		{

		}
	}
end

-- What this means is that you can use Lua as a data description
-- that is very human-readable using nested tables
-- and ordered lists (arrays) and dictionary string keys:

local myData =
{
	id = 112358,
	names = { first = "Fred", last = "Fredson" },
	aliases = { "Freddy", "Freddie", "The Fredster", "Freddo" },
	address =
	{
		street = "1248 Fred Avenue",
		city = "Fredville",
		state = "Fredonia",
		zip = 87654
	},
	hobbies =
	{
		{
			name = "Computer games",
			priority = 5,
			isSkilled = true,
		},
		{
			name = "Jogging",
			priority = 1,
			isSkilled = false,
		},
		{
			name = "Sleeping",
			priority = 10,
			isSkilled = true,
		},
	},
}

-- Note that trailing commas in table constructors are ok:
--	myTable = { 1, 2, 3, }
--	myTable = { name = "Fred", age = 123, }

-- In our class we will use Lua tables as a file format
-- for most of the authored assets
-- because it is easy to read and easy to author.
