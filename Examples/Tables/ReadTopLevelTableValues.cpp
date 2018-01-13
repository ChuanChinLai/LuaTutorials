// Include Files
//==============

#include "ReadTopLevelTableValues.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Results/Results.h>
#include <External/Lua/Includes.h>
#include <iostream>
#include <string>

// Helper Function Declarations
//=============================

namespace
{
	eae6320::cResult LoadTableValues( lua_State& io_luaState );
	eae6320::cResult LoadTableValues_stringKeys( lua_State& io_luaState );
	eae6320::cResult LoadTableValues_integerKeys( lua_State& io_luaState );
	eae6320::cResult LoadTableValues_allKeys( lua_State& io_luaState );

	eae6320::cResult LoadAsset( const char* const i_path );
}

// Interface
//==========

eae6320::cResult ReadTopLevelTableValues()
{
	// The LoadAsset() function does _exactly_ what was shown
	// in the LoadTableFromFile examples.
	// After the table is loaded at the top of the stack, though,
	// LoadTableValues() is called,
	// so that is the function you should pay attention to.

	auto result = eae6320::Results::Success;

	constexpr auto* const path = "readTopLevelTableValues.lua";
	if ( !( result = LoadAsset( path ) ) )
	{
		return result;
	}

	return result;
}

// Helper Function Definitions
//============================

namespace
{
	eae6320::cResult LoadTableValues( lua_State& io_luaState )
	{
		auto result = eae6320::Results::Success;

		// You will need to know how to load string and integer keys for this class:
		if ( !( result = LoadTableValues_stringKeys( io_luaState ) ) )
		{
			return result;
		}
		if ( !( result= LoadTableValues_integerKeys( io_luaState ) ) )
		{
			return result;
		}

		// You do _not_ need to know how to iterate through all keys for Assignment 02,
		// but you may want to look at this example at a later for a future assignment:
		if ( !( result = LoadTableValues_allKeys( io_luaState ) ) )
		{
			return result;
		}

		return result;
	}

	eae6320::cResult LoadTableValues_stringKeys( lua_State& io_luaState )
	{
		auto result = eae6320::Results::Success;

		// Get the value of "name"
		{
			// Remember in Lua you access a table's value like this:
			//	v = t[k]
			// where t is the table, k is the key, and v is the value

			// In C/C++ code you access a table's value like this pseudocode:
			//	lua_push( lua_State*, k );
			//	lua_gettable( lua_State*, t );
			//	v = lua_tovalue( lua_State*, -1 );
			// where t is the _index_ of the table.
			// In words, you:
			//	* Push a table on the stack
			//	* Push the key on the top of the stack
			//	* Call lua_gettable(), providing the index of the table
			//		(the index of the key is required to be -1)
			//	* lua_gettable() will pop the key, and push the value.
			//		In other words, everything on the stack will be as it was before the call
			//		except the key will have been replaced with a value
			//		(which will be nil if you asked for a key that doesn't exist in the table).

			// Let's do it for real, and be clear at every step what the stack looks like:

			// Right now, the stack only has _one_ object,
			// which is the table from the asset file, at index -1.
			// We want to look up the value associated with the key "name":
			constexpr auto* const key = "name";
			lua_pushstring( &io_luaState, key );

			// Now, the key is at index -1
			// and the table is at index -2.
			// (Does that make sense why the table is at -2?
			// I think of it like "the table was on top,
			// but when I pushed something new on the top it got pushed down".)

			// We can now call lua_gettable() to get the value associated
			// with the key at index -1:
			{
				constexpr int currentIndexOfTable = -2;
				lua_gettable( &io_luaState, currentIndexOfTable );
			}

			// Now, the value that was at t[k] is at index -1
			// and the table is still at index -2.

			// The following situations are possible:
			//	* There _was_ a value at the key
			//		* It may be the value type that we expect (a string, in this example)
			//		* It may be a different value type (like a boolean, in our current example)
			//	* There was no value at the key

			// It is up to you how explicit you want to make your error checking.
			// In general, writing more error checking code is annoying to do
			// and makes the code harder to read,
			// but it makes life easier for the users of your software
			// (and for you, in cases where something that "should never happen" does.

			// In this first example we will do as much error checking as we can,
			// and then in the next example we will not do any.
			// This will allow you to see both and decide how much you personally prefer.

			// First, then, we will make sure that a value (_any_ value) existed for the key:
			if ( lua_isnil( &io_luaState, -1 ) )
			{
				result = eae6320::Results::InvalidFile;
				std::cerr << "No value for \"" << key << "\" was found in the asset table" << std::endl;
				// When using Lua in C/C++ it is important
				// to always return the stack to its original state.
				// Leaving objects on the stack is kind of like a Lua memory leak.
				// Remember that our value is currently at -1 and the table is at -2;
				// In order to restore it to the way it was when we entered this function
				// we need to pop the value off the stack:
				lua_pop( &io_luaState, 1 );
				// Now the only thing on the stack is the asset table at -1,
				// and the calling function will deal with this
				// (regardless of whether this function succeeds or fails).
				return result;
			}

			// If we reach this code then we know that a value exists,
			// but we don't know if it's the right type or not.
			// One way we could find out in the current example is this:
			//	if ( lua_isstring( &io_luaState, -1 ) )
			// This would work (and is perfectly valid),
			// but it actually returns true if the value is a number
			// (because numbers are always convertible to strings).
			// If we really want to be strict, we can do the following instead:
			if ( lua_type( &io_luaState, -1 ) != LUA_TSTRING )
			{
				result = eae6320::Results::InvalidFile;
				std::cerr << "The value for \"" << key << "\" must be a string "
					"(instead of a " << luaL_typename( &io_luaState, -1 ) << ")" << std::endl;
				// Pop the value
				lua_pop( &io_luaState, 1 );
				// (The asset table is now at -1)
				return result;
			}

			// Now we know that the value stored in the table at the key is valid:
			{
				const auto* const value = lua_tostring( &io_luaState, -1 );
				// You can now do whatever you want with the value.
				// NOTE! The string is a pointer to a char array,
				// which means that Lua owns the memory.
				// You should do whatever you need with the value
				// _before_ popping the value from the stack.
				// If you can't use the value right away,
				// then either copy it to your own char array or store it in a std::string.
				// (THIS IS ONE OF THE MOST COMMON MISTAKES DEALING WITH LUA IN THIS CLASS!)
				const std::string myCopyOfTheValue = value;
				std::cout << "The value for \"" << key << "\" is \"" << value << "\"" << std::endl;
				// Pop the value
				lua_pop( &io_luaState, 1 );
				// (The asset table is now at -1)
			}
		}

		// Get the value of "age"
		{
			// This time we will get the value with no error checking.
			// Remember that right now the asset table is at index -1;
			// see if you can follow what happens to the stack in your head:
			constexpr auto* const key = "age";
			lua_pushstring( &io_luaState, key );
			lua_gettable( &io_luaState, -2 );
			const auto value = lua_tonumber( &io_luaState, -1 );
			lua_pop( &io_luaState, 1 );	// Unlike a string, a number can be popped immediately

			std::cout << "The value for \"" << key << "\" is " << value << std::endl;
		}

		return result;
	}

	eae6320::cResult LoadTableValues_integerKeys( lua_State& io_luaState )
	{
		auto result = eae6320::Results::Success;

		// Load the first value with an explicit knowledge of its integer key
		{
			// This first example assumes that you know exactly how many
			// ordered values to expect
			// (or, at least, that you know there is at least one).

			// It is the same as the string example,
			// except that it uses an integer as a key
			// (remember that right now the asset table is at index -1):
			constexpr int key = 1;
			lua_pushinteger( &io_luaState, key );
			lua_gettable( &io_luaState, -2 );
			const auto* const value = lua_tostring( &io_luaState, -1 );
			if ( value )
			{
				std::cout << "The value at key " << key << " is \"" << value << "\"" << std::endl;
				lua_pop( &io_luaState, 1 );	// Pop the string
			}
			else
			{
				result = eae6320::Results::InvalidFile;
				std::cerr << "There is no string value at key " << key << std::endl;
				lua_pop( &io_luaState, 1 );	// Pop the nil
				return result;
			}
		}

		// Iterate through all values in the array
		{
			// If you know that the table is an array
			// (i.e. it contains an ordered list)
			// but you don't know how many values to expect
			// you can iterate through it by getting the length of the table
			// (luaL_len() is equivalent to the # operator in Lua):
			const auto arrayLength = luaL_len( &io_luaState, -1 );
			if ( arrayLength > 0 )
			{
				std::cout << "Iterating through the ordered values:" << std::endl;
				// Remember that Lua arrays are 1-based and not 0-based!
				for ( int i = 1; i <= arrayLength; ++i )
				{
					lua_pushinteger( &io_luaState, i );
					lua_gettable( &io_luaState, -2 );
					const auto* const value = lua_tostring( &io_luaState, -1 );
					if ( value )
					{
						std::cout << "\tThe value #" << i << " is \"" << value << "\"" << std::endl;
						lua_pop( &io_luaState, 1 );
					}
					else
					{
						result = eae6320::Results::InvalidFile;
						std::cerr << "\tThe value #" << i << " isn't a string!" << std::endl;
						lua_pop( &io_luaState, 1 );
						return result;
					}
				}
			}
			else
			{
				result = eae6320::Results::InvalidFile;
				std::cerr << "The asset table doesn't have any ordered values" << std::endl;
				return result;
			}
		}

		return result;
	}

	eae6320::cResult LoadTableValues_allKeys( lua_State& io_luaState )
	{
		// YOU DO NOT NEED TO KNOW HOW TO DO WHAT THIS EXAMPLE DOES RIGHT NOW!
		// It is here if you're curious,
		// but if you're not or if you're feeling overwhelmed
		// you don't have to read any further.

		// You may recall that Lua has a pairs() function
		// that allows you to iterate over _every_ key/value pair
		// in a table, like this:
		//	for key, value in pairs( someTable ) do
		//		-- something
		//	end

		// You can do the same thing in C/C++ code by doing the following:
		std::cout << "Iterating through every key/value pair in the table:" << std::endl;
		lua_pushnil( &io_luaState );	// This tells lua_next() to return the first key
		constexpr int tableIndex = -2;
		while ( lua_next( &io_luaState, tableIndex ) )
		{
			// After lua_next() the key is at -2 and the value is at -1:
			if ( lua_type( &io_luaState, -2 ) == LUA_TSTRING )
			{
				std::cout << "\tKey = " << lua_tostring( &io_luaState, -2 );
			}
			else
			{
				std::cout << "\tKey = " << lua_tonumber( &io_luaState, -2 );
			}
			if ( lua_type( &io_luaState, -1 ) == LUA_TSTRING )
			{
				std::cout << "\t\tValue = " << lua_tostring( &io_luaState, -1 ) << std::endl;
			}
			else
			{
				std::cout << "\t\tValue = " << lua_tonumber( &io_luaState, -1 ) << std::endl;
			}

			// To iterate to the next key/value pair, pop the value, but leave the key:
			lua_pop( &io_luaState, 1 );

			// One important subtlety to note:
			// You can't change the table while iterating using lua_next(),
			// but calling lua_tostring() on a number does just that.
			// That's why I'm checking the types before printing them out.
			// Again, you don't need to worry about this for Assignment 03,
			// and for our class I think you will always know what data type you are expecting,
			// but if you're curious you can read more here:
			//	http://www.lua.org/manual/5.3/manual.html#lua_next
		}

		return eae6320::Results::Success;
	}

	eae6320::cResult LoadAsset( const char* const i_path )
	{
		auto result = eae6320::Results::Success;

		// Create a new Lua state
		lua_State* luaState = nullptr;
		{
			luaState = luaL_newstate();
			if ( !luaState )
			{
				result = eae6320::Results::OutOfMemory;
				std::cerr << "Failed to create a new Lua state" << std::endl;
				goto OnExit;
			}
		}

		// Load the asset file as a "chunk",
		// meaning there will be a callable function at the top of the stack
		const auto stackTopBeforeLoad = lua_gettop( luaState );
		{
			const auto luaResult = luaL_loadfile( luaState, i_path );
			if ( luaResult != LUA_OK )
			{
				result = eae6320::Results::Failure;
				std::cerr << lua_tostring( luaState, -1 ) << std::endl;
				// Pop the error message
				lua_pop( luaState, 1 );
				goto OnExit;
			}
		}
		// Execute the "chunk", which should load the asset
		// into a table at the top of the stack
		{
			constexpr int argumentCount = 0;
			constexpr int returnValueCount = LUA_MULTRET;	// Return _everything_ that the file returns
			constexpr int noMessageHandler = 0;
			const auto luaResult = lua_pcall( luaState, argumentCount, returnValueCount, noMessageHandler );
			if ( luaResult == LUA_OK )
			{
				// A well-behaved asset file will only return a single value
				const auto returnedValueCount = lua_gettop( luaState ) - stackTopBeforeLoad;
				if ( returnedValueCount == 1 )
				{
					// A correct asset file _must_ return a table
					if ( !lua_istable( luaState, -1 ) )
					{
						result = eae6320::Results::InvalidFile;
						std::cerr << "Asset files must return a table (instead of a "
							<< luaL_typename( luaState, -1 ) << ")" << std::endl;
						// Pop the returned non-table value
						lua_pop( luaState, 1 );
						goto OnExit;
					}
				}
				else
				{
					result = eae6320::Results::InvalidFile;
					std::cerr << "Asset files must return a single table (instead of "
						<< returnedValueCount << " values)" << std::endl;
					// Pop every value that was returned
					lua_pop( luaState, returnedValueCount );
					goto OnExit;
				}
			}
			else
			{
				result = eae6320::Results::InvalidFile;
				std::cerr << lua_tostring( luaState, -1 );
				// Pop the error message
				lua_pop( luaState, 1 );
				goto OnExit;
			}
		}

		// If this code is reached the asset file was loaded successfully,
		// and its table is now at index -1
		result = LoadTableValues( *luaState );

		// Pop the table
		lua_pop( luaState, 1 );

	OnExit:

		if ( luaState )
		{
			// If I haven't made any mistakes
			// there shouldn't be anything on the stack,
			// regardless of any errors encountered while loading the file:
			EAE6320_ASSERT( lua_gettop( luaState ) == 0 );

			lua_close( luaState );
			luaState = nullptr;
		}

		return result;
	}
}
