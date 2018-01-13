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
		const auto result = luaL_dofile( luaState, "luaFunctionsFromC.lua" );
		if ( result != LUA_OK )
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

	// ExampleFunction()
	{
		// To call a function it must be pushed onto the stack:
		lua_getglobal( luaState, "ExampleFunction" );
		// This function has no arguments and doesn't return any values
		constexpr int argumentCount = 0;
		constexpr int returnValueCount = 0;
		lua_call( luaState, argumentCount, returnValueCount );
		// Notice that calling this function will display a message
	}

	// ExamplePrint()
	{
		lua_getglobal( luaState, "ExamplePrint" );
		// This function has a single argument
		constexpr int argumentCount = 1;
		{
			// Function arguments must be pushed onto the stack
			// after the function is and in order
			constexpr char* const stringToPrint = "Example string to print";
			lua_pushstring( luaState, stringToPrint );
		}
		constexpr int returnValueCount = 0;
		lua_call( luaState, argumentCount, returnValueCount );
	}
	// Since the Lua function uses tostring()
	// other variable types besides "string" can be passed as arguments:
	{
		lua_getglobal( luaState, "ExamplePrint" );
		constexpr int argumentCount = 1;
		{
			constexpr lua_Number numberToPrint = 123.456;
			lua_pushnumber( luaState, numberToPrint );
		}
		constexpr int returnValueCount = 0;
		lua_call( luaState, argumentCount, returnValueCount );
	}

	// ExampleDouble()
	{
		constexpr lua_Number number_toDouble = 12.34;
		lua_Number number_doubled;
		{
			lua_getglobal( luaState, "ExampleDouble" );
			constexpr int argumentCount = 1;
			{
				lua_pushnumber( luaState, number_toDouble );
			}
			// This function returns a single value
			constexpr int returnValueCount = 1;
			lua_call( luaState, argumentCount, returnValueCount );
			{
				// Lua pushes return values onto the stack in order
				number_doubled = lua_tonumber( luaState, -1 );
				// The C/C++ program is responsible for popping the returned values
				// off the stack when it is done with them
				lua_pop( luaState, returnValueCount );
			}
		}
		std::cout<< "The result of doubling " << number_toDouble
			<< " using ExampleDouble() is: " << number_doubled << std::endl;
	}

	// ExampleAdd()
	{
		constexpr lua_Number numberToAdd1 = 1.0;
		constexpr lua_Number numberToAdd2 = 2.0;
		lua_Number sum;
		{
			lua_getglobal( luaState, "ExampleAdd" );
			constexpr int argumentCount = 2;
			{
				lua_pushnumber( luaState, numberToAdd1 );
				lua_pushnumber( luaState, numberToAdd2 );
			}
			constexpr int returnValueCount = 1;
			lua_call( luaState, argumentCount, returnValueCount );
			{
				sum = lua_tonumber( luaState, -1 );
				lua_pop( luaState, returnValueCount );
			}
		}
		std::cout<< "The result of adding " << numberToAdd1 << " + " << numberToAdd2
			<< " using ExampleAdd() is: " << sum << std::endl;
	}

	// ExampleStats()
	{
		constexpr lua_Number number1 = 1.2;
		constexpr lua_Number number2 = 3.4;
		constexpr lua_Number number3 = 5.6;
		constexpr lua_Number number4 = 7.8;
		lua_Number sum, product, average;
		{
			lua_getglobal( luaState, "ExampleStats" );
			constexpr int argumentCount = 4;
			{
				lua_pushnumber( luaState, number1 );
				lua_pushnumber( luaState, number2 );
				lua_pushnumber( luaState, number3 );
				lua_pushnumber( luaState, number4 );
			}
			constexpr int returnValueCount = 3;
			lua_call( luaState, argumentCount, returnValueCount );
			{
				// Positive stack indices refer to the bottom of the stack,
				// negative stack indices refer to the top of the stack.
				// Lua pushes return values to the top of the stack,
				// and so the negative indices required to access
				// multiple return values in the proper order
				// may be backwards from what you expect:
				//	* The sum is pushed first, and is initially -1
				//	* The product is pushed next, which makes it -1 and the sum -2
				//	* The average is pushed last, which makes it -1, the product -2, and the sum -3
				sum = lua_tonumber( luaState, -3 );
				product = lua_tonumber( luaState, -2 );
				average = lua_tonumber( luaState, -1 );
				lua_pop( luaState, returnValueCount );
			}
		}
		std::cout << "Given "
			<< number1 << ", " << number2 << ", " << number3 << ", and " << number4
			<< ", ExampleStats() returns:\n"
			"\tsum = " << sum << ", product = " << product << ", average = " << average << std::endl;
	}

	// ExampleError()
	{
		lua_getglobal( luaState, "ExampleError" );
		// Using lua_call() to call Lua functions doesn't account for run-time errors.
		// If there _is_ a Lua error during lua_call() then your program will exit with an error code,
		// which is usually not what you want (although it can be in some cases).
		// If you want to catch the error and handle it yourself
		// you should use lua_pcall(), where the "p" stands for "protected".
		// lua_pcall() will return the relevant error code to you
		// as well as letting you handle the error yourself.
		constexpr int argumentCount = 0;
		constexpr int returnValueCount = 0;
		// If you want to get fancy you can create your own error handler function.
		// In our class we will always use the default Lua error handler
		// which means that the 4th parameter to lua_pcall() will always be 0.
		constexpr int noErrorHandler = 0;
		const auto result = lua_pcall( luaState, argumentCount, returnValueCount, noErrorHandler );
		if ( result != LUA_OK )
		{
			// If there was an error
			// the default Lua message handler will push
			// the relevant error message to the top of the stack
			const auto* const errorMessage = lua_tostring( luaState, -1 );
			std::cerr << errorMessage << std::endl;
			lua_pop( luaState, 1 );
		}
	}

	// ExampleErrorChecking()
	{
		// This function expects a string as argument #1

		// First it will be called correctly:
		{
			lua_getglobal( luaState, "ExampleErrorChecking" );
			constexpr int argumentCount = 1;
			{
				constexpr char* const stringArgument = "This is the correct type";
				lua_pushstring( luaState, stringArgument );
			}
			constexpr int returnValueCount = 0;
			constexpr int noErrorHandler = 0;
			const auto result = lua_pcall( luaState, argumentCount, returnValueCount, noErrorHandler );
			if ( result != LUA_OK )
			{
				const auto* const errorMessage = lua_tostring( luaState, -1 );
				std::cerr << errorMessage << std::endl;
				lua_pop( luaState, 1 );
			}
		}
		// Now it will be called incorrectly
		// (notice the difference in output):
		{
			lua_getglobal( luaState, "ExampleErrorChecking" );
			constexpr int argumentCount = 1;
			{
				constexpr bool booleanArgument = false;
				lua_pushboolean( luaState, booleanArgument );
			}
			constexpr int returnValueCount = 0;
			constexpr int noErrorHandler = 0;
			const auto result = lua_pcall( luaState, argumentCount, returnValueCount, noErrorHandler );
			if ( result != LUA_OK )
			{
				const auto* const errorMessage = lua_tostring( luaState, -1 );
				std::cerr << errorMessage << std::endl;
				lua_pop( luaState, 1 );
			}
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
