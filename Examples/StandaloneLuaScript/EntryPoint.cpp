/*
	The main() function is where the program starts execution
*/

// Include Files
//==============

#include <cstdlib>
#include <Engine/Asserts/Asserts.h>
#include <External/Lua/Includes.h>
#include <iostream>

// Entry Point
//============

int main( int i_argumentCount, char** i_arguments )
{
	int exitCode = EXIT_SUCCESS;

	// Create a new Lua state
	lua_State* luaState = nullptr;
	{
		luaState = luaL_newstate();
		if ( !luaState )
		{
			exitCode = EXIT_FAILURE;
			goto OnExit;
		}
	}
	// Open the standard libraries
	luaL_openlibs( luaState );

	// Load and run the stand-alone Lua script
	{
		const auto result = luaL_dofile( luaState, "standaloneScript.lua" );
		if ( result == LUA_OK )
		{
			// If there were no errors then the Lua script will have printed things to the console window.
			// You should read through the code while looking at the output to make sure you understand.
		}
		else
		{
			// If there was an error
			// an error message will be at the top of the stack
			const auto* const errorMessage = lua_tostring( luaState, -1 );
			std::cerr << errorMessage << std::endl;
			lua_pop( luaState, 1 );

			exitCode = EXIT_FAILURE;
			goto OnExit;
		}
	}

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

	return exitCode;
}
