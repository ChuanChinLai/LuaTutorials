// Include Files
//==============

#include "LoadTableFromFile.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Results/Results.h>
#include <External/Lua/Includes.h>
#include <iostream>

// Helper Function Declarations
//=============================

namespace
{
	eae6320::cResult LoadAsset_method1( const char* const i_path );
	eae6320::cResult LoadAsset_method2( const char* const i_path );
	eae6320::cResult LoadAsset_hybridMethod( const char* const i_path );
}

// Interface
//==========

eae6320::cResult LoadTableFromFile()
{
	// In our class an asset file using Lua as a format
	// must _always_ conform to two rules:
	//	* It must be a single table
	//	* It must return itself from the file
	// See loadTableFromFile.lua for an example.

	// There are two ways you can choose to load one of these files:
	//	1) Using luaL_dofile()
	//	2) Using luaL_loadfile() and then lua_pcall()

	// #1 does loads the file _and_ executes it in one function call.
	// It pushes all return values on the stack, however.
	// This means that if you load a file that isn't "well-behaved"
	// then it could return more than one value,
	// and you need to check for this to make sure that
	// your stack can get restored to where it was before.

	// #2 requires you to do two steps and check for errors at both,
	// but since you specify that you only want one return value
	// you don't have to check for that.

	// Which method you choose is up to you.
	// You could even do a hybrid and follow #2
	// but pass LUA_MULTRET for the number of arguments to lua_pcall()
	// (which is what #1 does)
	// if you wanted to do the maximum amount of error checking.

	// This file shows all three methods:
	
	auto result = eae6320::Results::Success;

	constexpr auto* const path = "loadTableFromFile.lua";
	if ( !( result = LoadAsset_method1( path ) ) )
	{
		return result;
	}
	if ( !( result = LoadAsset_method2( path ) ) )
	{
		return result;
	}
	if ( !( result = LoadAsset_hybridMethod( path ) ) )
	{
		return result;
	}

	return result;
}

// Helper Function Definitions
//============================

namespace
{
	eae6320::cResult LoadAsset_method1( const char* const i_path )
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

		// Load the asset file into a table at the top of the stack
		{
			const auto stackTopBeforeLoading = lua_gettop( luaState );
			const auto luaResult = luaL_dofile( luaState, i_path );
			if ( luaResult == LUA_OK )
			{
				// A well-behaved asset file will only return a single value
				const auto returnedValueCount = lua_gettop( luaState ) - stackTopBeforeLoading;
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
				result = eae6320::Results::Failure;
				std::cerr << lua_tostring( luaState, -1 ) << std::endl;
				// Pop the error message
				lua_pop( luaState, 1 );
				goto OnExit;
			}
		}

		// If this code is reached the asset file was loaded successfully,
		// and its table is now at index -1
		lua_pop( luaState, 1 );

	OnExit:

		if ( luaState )
		{
			// If I haven't made any mistakes
			// there shouldn't be anything on the stack
			// regardless of any errors
			EAE6320_ASSERT( lua_gettop( luaState ) == 0 );

			lua_close( luaState );
			luaState = nullptr;
		}

		return result;
	}

	eae6320::cResult LoadAsset_method2( const char* const i_path )
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
			// Right now, the chunk is at index -1
			// (that's what luaL_loadfile() does)
			constexpr int argumentCount = 0;
			constexpr int returnValueCount = 1;	// We expect an asset table to be returned
			constexpr int noErrorHandler = 0;
			const auto luaResult = lua_pcall( luaState, argumentCount, returnValueCount, noErrorHandler );
			if ( luaResult == LUA_OK )
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
				std::cerr << lua_tostring( luaState, -1 ) << std::endl;
				// Pop the error message
				lua_pop( luaState, 1 );
				goto OnExit;
			}
		}

		// If this code is reached the asset file was loaded successfully,
		// and its table is now at index -1
		lua_pop( luaState, 1 );

	OnExit:

		if ( luaState )
		{
			// If I haven't made any mistakes
			// there shouldn't be anything on the stack
			// regardless of any errors
			EAE6320_ASSERT( lua_gettop( luaState ) == 0 );

			lua_close( luaState );
			luaState = nullptr;
		}

		return result;
	}

	eae6320::cResult LoadAsset_hybridMethod( const char* const i_path )
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
				eae6320::Results::Failure;
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
			constexpr int noErrorHandler = 0;
			const auto luaResult = lua_pcall( luaState, argumentCount, returnValueCount, noErrorHandler );
			if ( luaResult == LUA_OK )
			{
				// A well-behaved asset file will only return a single value
				const int returnedValueCount = lua_gettop( luaState ) - stackTopBeforeLoad;
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
				std::cerr << lua_tostring( luaState, -1 ) << std::endl;
				// Pop the error message
				lua_pop( luaState, 1 );
				goto OnExit;
			}
		}

		// If this code is reached the asset file was loaded successfully,
		// and its table is now at index -1
		lua_pop( luaState, 1 );

	OnExit:

		if ( luaState )
		{
			// If I haven't made any mistakes
			// there shouldn't be anything on the stack
			// regardless of any errors
			EAE6320_ASSERT( lua_gettop( luaState ) == 0 );

			lua_close( luaState );
			luaState = nullptr;
		}

		return result;
	}
}
