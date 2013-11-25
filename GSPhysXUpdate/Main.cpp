/// \file 3DGamestudio physX update example

#include "Common.h"
#include "PhysX3Util.h"

/// \brief App entry point
///
/// \return int App return code
int main()
{
	// Open game engine
	ENGINE_VARS *ev = engine_open("");

	av(fps_max) = _VAR(30);

	// Init physX
	PhysX3Util *physX3 = new PhysX3Util();
	physX3->Init();

	// Create empty level
	level_load("");
	// place camera
	vec_set((VECTOR *)&(av(camera)).x, vector(0, 0, _VAR(1000)));
	av(camera).tilt = _VAR(-90);

	// Create kinematic actor
	ENTITY *kinematic = ent_create("CUBE.MDL", vector(0, 0, 0), NULL);
	physX3->CreateKinematic(kinematic);

	// FPS
	float fps = 0;

	// Engine loop
	while(engine_frame())
	{
		// Step physX
		physX3->Run(_FLOAT(av(time_frame)));

		// Draw FPS
		fps = 0.9f * fps + 0.1f / _FLOAT(av(time_frame)) * 16;
		draw_text(_chr(str_printf(NULL, "FPS : %d", (int)fps)), _VAR(0), _VAR(0), (COLOR *)vector(0, 0, _VAR(255)));
	}

	physX3->Release();

	// Quit game engine
	engine_close();
}