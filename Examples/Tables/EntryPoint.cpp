/*
	The main() function is where the program starts execution
*/

// Include Files
//==============

#include "LoadTableFromFile.h"
#include "ReadNestedTableValues.h"
#include "ReadTopLevelTableValues.h"

#include <cstdlib>
#include <Engine/Results/Results.h>

// Entry Point
//============

int main( int i_argumentCount, char** i_arguments )
{
	// How to load an asset using a Lua table as its file format
	if ( !LoadTableFromFile() )
	{
		return EXIT_FAILURE;
	}

	// How to read basic values from an asset table
	if ( !ReadTopLevelTableValues() )
	{
		return EXIT_FAILURE;
	}

	// How to read tables within an asset table
	if ( !ReadNestedTableValues() )
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
