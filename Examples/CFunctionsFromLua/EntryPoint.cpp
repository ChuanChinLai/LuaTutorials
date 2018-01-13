/*
	The main() function is where the program starts execution
*/

// Include Files
//==============

#include <cstdlib>
#include <Engine/Asserts/Asserts.h>
#include <External/Lua/Includes.h>
#include <iostream>

// Helper Function Declarations
//=============================

namespace
{
	// For a C/C++ function to be callable by Lua it must be a lua_CFunction:
	//	* typedef int (*lua_CFunction)( lua_State *L );
	// The input parameters will already be pushed on the stack in the provided lua_State*
	// and the function should push the return values on the stack
	// and then return how many there are.
	// There will be more explanation in the examples.

	int ExampleFunction( lua_State* io_luaState );
	int ExamplePrint( lua_State* io_luaState );
	int ExampleDouble( lua_State* io_luaState );
	int ExampleAdd( lua_State* io_luaState );
	int ExampleStats( lua_State* io_luaState );
	int ExampleError( lua_State* io_luaState );
	int ExampleErrorChecking( lua_State* io_luaState );
}

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

	// Make the C/C++ functions available to the Lua state
	// (this sets the function as a global variable
	// using the name provided)
	{
		lua_register( luaState, "ExampleFunction", ExampleFunction );
		lua_register( luaState, "ExamplePrint", ExamplePrint );
		lua_register( luaState, "ExampleDouble", ExampleDouble );
		lua_register( luaState, "ExampleAdd", ExampleAdd );
		lua_register( luaState, "ExampleStats", ExampleStats );
		lua_register( luaState, "ExampleError", ExampleError );
		lua_register( luaState, "ExampleErrorChecking", ExampleErrorChecking );
	}

	// Load and run the Lua script that calls the functions
	{
		// If you set breakpoints in the registered C/C++ functions
		// before executing the Lua script you will be able to watch them get called
		const auto result = luaL_dofile( luaState, "CFunctionsFromLua.lua" );
		if ( result != LUA_OK )
		{
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
		luaState = NULL;
	}

	return exitCode;
}

// Helper Function Definitions
//============================

namespace
{
	int ExampleFunction( lua_State* io_luaState )
	{
		// This function doesn't use any input parameters:
		std::cout << "This statement will be displayed when this function is called" << std::endl;

		// This function doesn't return any values:
		constexpr int returnValueCount = 0;
		return returnValueCount;
	}

	int ExamplePrint( lua_State* io_luaState )
	{
		// This function expects a single input parameter.

		// Every argument that the Lua script provides as input to the function
		// is available to us here in our C/C++ function
		// as values pushed on the stack in the provided lua_State.
		// These values have _positive_ indices.
		// For example, if a function were called from Lua like this:
		//	* SomeFunction( argument1, argument2, argument3 )
		// Then in the SomeFunction() C/C++ code argument1 would be index 1,
		// argument2 would be index 2, and argument3 would be index 3.

		// In the case of this specific function, then,
		// we expect that the caller has provided a single argument,
		// and that argument should be at index 1:
		const auto* const i_value = lua_tostring( io_luaState, 1 );

		std::cout << "The input parameter to ExamplePrint() is " << i_value << std::endl;	

		// This function doesn't return any values:
		constexpr int returnValueCount = 0;
		return returnValueCount;
	}

	int ExampleDouble( lua_State* io_luaState )
	{
		// This function expects a single input parameter:
		const auto i_value = lua_tonumber( io_luaState, 1 );

		// This function has a single return value
		const auto o_value = i_value + i_value;

		// Values to return should be pushed on the stack in order.
		// For example, if a function were called from Lua like this:
		//	* returnValue1, returnValue2, returnValue3 = SomeFunction()
		// Then in the SomeFunction() C/C++ code returnValue1 would be pushed first,
		// returnValue2 pushed next, and returnValue3 pushed last.

		// In the case of this specific function, then,
		// we push the return value:
		lua_pushnumber( io_luaState, o_value );

		// And return how many values that we pushed:
		constexpr int returnValueCount = 1;
		return returnValueCount;
	}

	int ExampleAdd( lua_State* io_luaState )
	{
		// This function expects multiple input parameters:
		const auto i_value1 = lua_tonumber( io_luaState, 1 );
		const auto i_value2 = lua_tonumber( io_luaState, 2 );

		const auto o_value = i_value1 + i_value2;
		lua_pushnumber( io_luaState, o_value );
		constexpr int returnValueCount = 1;
		return returnValueCount;
	}

	int ExampleStats( lua_State* io_luaState )
	{
		const auto i_value1 = lua_tonumber( io_luaState, 1 );
		const auto i_value2 = lua_tonumber( io_luaState, 2 );
		const auto i_value3 = lua_tonumber( io_luaState, 3 );
		const auto i_value4 = lua_tonumber( io_luaState, 4 );

		// This function has multiple return values:
		const auto o_sum = i_value1 + i_value2 + i_value3 + i_value4;
		const auto o_product = i_value1 * i_value2 * i_value3 * i_value4;
		const auto o_average = o_sum / 4.0;

		lua_pushnumber( io_luaState, o_sum );
		lua_pushnumber( io_luaState, o_product );
		lua_pushnumber( io_luaState, o_average );

		constexpr int returnValueCount = 3;
		return returnValueCount;
	}

	int ExampleError( lua_State* io_luaState )
	{
		// You don't need to understand what the following line does;
		// it is an illegal call (like dereferencing a NULL pointer in C/C++)
		// and will cause Lua to throw an error
		lua_gettable( io_luaState, 1 );

		// After the previous statement this function will jump to a Lua error handler
		// (like when a C++ exception is thrown)
		// and any code that follows won't actually be executed

		constexpr int returnValueCount = 0;
		return returnValueCount;
	}

	int ExampleErrorChecking( lua_State* io_luaState )
	{
		// This hypothetical function expects a string for an argument
		const auto type = lua_type( io_luaState, 1 );
		if ( type == LUA_TSTRING )
		{
			std::cout << "A string was correctly passed as argument #1 to ExampleErrorChecking()" << std::endl;
		}
		else
		{
			// You can throw an error from C just like you can in Lua using the error() function:
			return luaL_error( io_luaState,
				"A %s was incorrectly passed as argument #1 to ExampleErrorChecking() "
				"(instead of a string)", lua_typename( io_luaState, type ) );
			// Note that neither lua_error() nor luaL_error() actually return;
			// they jump to a Lua error handler just like what is discussed
			// in the comments of the ExampleError() function.
			// It is idiomatic Lua to return the result as shown above,
			// but if you step through the code in a debugger
			// you will see that the function doesn't actually return.
		}

		constexpr int returnValueCount = 0;
		return returnValueCount;
	}
}
