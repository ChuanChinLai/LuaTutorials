// Include Files
//==============

#include "ReadNestedTableValues.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Results/Results.h>
#include <External/Lua/Includes.h>
#include <iostream>

// Helper Function Declarations
//=============================

namespace
{
	eae6320::cResult LoadTableValues( lua_State& io_luaState );

	eae6320::cResult LoadTableValues_textures( lua_State& io_luaState );
	eae6320::cResult LoadTableValues_textures_paths( lua_State& io_luaState );

	eae6320::cResult LoadTableValues_parameters( lua_State& io_luaState );
	eae6320::cResult LoadTableValues_parameters_values( lua_State& io_luaState );

	eae6320::cResult LoadAsset( const char* const i_path );
}

// Interface
//==========

eae6320::cResult ReadNestedTableValues()
{
	// The LoadAsset() function does _exactly_ what was shown
	// in the LoadTableFromFile examples.
	// After the table is loaded at the top of the stack, though,
	// LoadTableValues() is called,
	// so that is the function you should pay attention to.

	auto result = eae6320::Results::Success;

	constexpr auto* const path = "readNestedTableValues.lua";
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

		if ( !( result = LoadTableValues_textures( io_luaState ) ) )
		{
			return result;
		}
		if ( !( result = LoadTableValues_parameters( io_luaState ) ) )
		{
			return result;
		}

		return result;
	}

	eae6320::cResult LoadTableValues_textures( lua_State& io_luaState )
	{
		auto result = eae6320::Results::Success;

		// Right now the asset table is at -1.
		// After the following table operation it will be at -2
		// and the "textures" table will be at -1:
		constexpr auto* const key = "textures";
		lua_pushstring( &io_luaState, key );
		lua_gettable( &io_luaState, -2 );
		// It can be hard to remember where the stack is at
		// and how many values to pop.
		// One strategy I would suggest is to always call a new function
		// When you are at a new level:
		// Right now we know that we have an original table at -2,
		// and a new one at -1,
		// and so we _know_ that we always have to pop at least _one_
		// value before leaving this function
		// (to make the original table be back to index -1).
		// If we don't do any further stack manipulation in this function
		// then it becomes easy to remember how many values to pop
		// because it will always be one.
		// This is the strategy I'll take in this example
		// (look at the "OnExit" label):
		if ( lua_istable( &io_luaState, -1 ) )
		{
			if ( !( result = LoadTableValues_textures_paths( io_luaState ) ) )
			{
				goto OnExit;
			}
		}
		else
		{
			result = eae6320::Results::InvalidFile;
			std::cerr << "The value at \"" << key << "\" must be a table "
				"(instead of a " << luaL_typename( &io_luaState, -1 ) << ")" << std::endl;
			goto OnExit;
		}

	OnExit:

		// Pop the textures table
		lua_pop( &io_luaState, 1 );

		return result;
	}

	eae6320::cResult LoadTableValues_textures_paths( lua_State& io_luaState )
	{
		// Right now the asset table is at -2
		// and the textures table is at -1.
		// NOTE, however, that it doesn't matter to me in this function
		// that the asset table is at -2.
		// Because I've carefully called a new function for every "stack level"
		// The only thing I care about is that the textures table that I care about
		// is at the top of the stack.
		// As long as I make sure that when I leave this function it is _still_
		// at -1 then it doesn't matter to me at all what is on the stack below it.

		auto result = eae6320::Results::Success;

		std::cout << "Iterating through every texture path:" << std::endl;
		const auto textureCount = luaL_len( &io_luaState, -1 );
		for ( int i = 1; i <= textureCount; ++i )
		{
			lua_pushinteger( &io_luaState, i );
			lua_gettable( &io_luaState, -2 );
			std::cout << "\t" << lua_tostring( &io_luaState, -1 ) << std::endl;
			lua_pop( &io_luaState, 1 );
		}

		return result;
	}

	eae6320::cResult LoadTableValues_parameters( lua_State& io_luaState )
	{
		auto result = eae6320::Results::Success;

		// Right now the asset table is at -1.
		// After the following table operation it will be at -2
		// and the "parameters" table will be at -1:
		constexpr auto* const key = "parameters";
		lua_pushstring( &io_luaState, key );
		lua_gettable( &io_luaState, -2 );
		if ( lua_istable( &io_luaState, -1 ) )
		{
			if ( !( result = LoadTableValues_parameters_values( io_luaState ) ) )
			{
				goto OnExit;
			}
		}
		else
		{
			result = eae6320::Results::InvalidFile;
			std::cerr << "The value at \"" << key << "\" must be a table "
				"(instead of a " << luaL_typename( &io_luaState, -1 ) << ")" << std::endl;
			goto OnExit;
		}

	OnExit:

		// Pop the parameters table
		lua_pop( &io_luaState, 1 );

		return result;
	}

	eae6320::cResult LoadTableValues_parameters_values( lua_State& io_luaState )
	{
		// Right now the parameters table is at -1.
		// Every time the while() statement is executed it will be at -2
		// and the next key will be at -1.
		// Inside the block the table will be at -3,
		// the current key will be at -2,
		// and the value will be at -1.
		// (You may want to review LoadTableValues_allKeys()
		// in the ReadTopLevelTableValues example,
		// but remember that you don't need to know how to do this
		// for Assignment 03.)

		auto result = eae6320::Results::Success;

		std::cout << "Iterating through the parameters:" << std::endl;
		lua_pushnil( &io_luaState );
		while ( lua_next( &io_luaState, -2 ) )
		{
			std::cout << "\t" << lua_tostring( &io_luaState, -2 )
				<< " = " << lua_tonumber( &io_luaState, -1 ) << std::endl;
			lua_pop( &io_luaState, 1 );
		}

		return result;
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
						std::cerr << "Asset files must return a table (instead of a " <<
							luaL_typename( luaState, -1 ) << ")" << std::endl;
						// Pop the returned non-table value
						lua_pop( luaState, 1 );
						goto OnExit;
					}
				}
				else
				{
					result = eae6320::Results::InvalidFile;
					std::cerr << "Asset files must return a single table (instead of " <<
						returnedValueCount << " values)" << std::endl;
					// Pop every value that was returned
					lua_pop( luaState, returnedValueCount );
					goto OnExit;
				}
			}
			else
			{
				result = eae6320::Results::InvalidFile;
				std::cerr << lua_tostring( luaState, -1 ) << std::endl;
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
