/// \file 3DGamestudio physX update example

#include "Common.h"

/// \brief App entry point
///
/// \return int App return code
int main()
{
	// Open game engine
	ENGINE_VARS *ev = engine_open("");

	av(fps_max) = _VAR(30);

	while(engine_frame())
	{
		printf("Frame %fs\n", _FLOAT(av(time_frame)) / 16);
	}

	// Quit game engine
	engine_close();
}