/// \file 3DGamestudio physX update example

#include "Common.h"
#include "PhysX3Util.h"

#define nullvector vector(0, 0, 0)

/// \brief App entry point
///
/// \return int App return code
int main()
{
	// Open game engine
	ENGINE_VARS *ev = engine_open("");

	// Help controls Panel
	TEXT *txtHelp = txt_create(_VAR(0), _VAR(1));
	txtHelp->flags |= SHOW;
	txtHelp->font = font_create("Arial#16");
	txt_addstring(txtHelp, "1 :\t\tUse Active transforms in update");
	txt_addstring(txtHelp, "2 :\t\tQuery all actors in update");
	txt_addstring(txtHelp, "3 :\t\tLimit FPS to 30");
	txt_addstring(txtHelp, "4 :\t\tLimit FPS to 60");
	txt_addstring(txtHelp, "5 :\t\tLimit FPS to Max");

	// Update mode
	TEXT *txtUpdate = txt_create(0, 1);
	txtUpdate->flags = SHOW;
	txtUpdate->font = font_create("Arial#20b");
	vec_set((VECTOR *)&(txtUpdate->blue), vector(0, _VAR(255), 0));
	txtUpdate->pos_x = _VAR(300);
	txt_addstring(txtUpdate, "Query actors");

	// Initial FPS
	av(fps_max) = _VAR(60);

	// Init physX
	PhysX3Util *physX3 = new PhysX3Util();
	physX3->Init();

	// Create empty level
	level_load("");
	// place camera
	vec_set((VECTOR *)&(av(camera)).x, vector(0, 0, _VAR(1000)));
	av(camera).tilt = _VAR(-90);

	// Create kinematic actor
	ENTITY *kinematicEnt = ent_create("CUBE.MDL", vector(0, 0, 0), NULL);
	physx::PxRigidDynamic *kine = physX3->CreateKinematic(kinematicEnt);

	// FPS
	float fps = 0;

	// Simple movement vectors
	VECTOR vecMove, vecForce, vecSpeed;
	vec_set(&vecMove, nullvector);
	vec_set(&vecForce, nullvector);
	vec_set(&vecSpeed, nullvector);

	// Engine loop
	while(av(key_esc) == 0 && engine_frame())
	{
		// Step physX
		physX3->Run(_FLOAT(av(time_frame)) / 16);

		// Get input
		float force_x = (_FLOAT(av(key_cuu)) - _FLOAT(av(key_cud))) * 6.0f;
		float force_y = (_FLOAT(av(key_cul)) - _FLOAT(av(key_cur))) * 6.0f;

		// Switch options
		if (av(key_1))
		{
			physX3->SetUseActiveTransforms(true);
			str_cpy(txtUpdate->pstring[0], "Active transforms");
		}
		else if (av(key_2))
		{
			physX3->SetUseActiveTransforms(false);
			str_cpy(txtUpdate->pstring[0], "Query Actors");
		}
		else if (av(key_3))
		{
			av(fps_max) = _VAR(30);
		}
		else if (av(key_4))
		{
			av(fps_max) = _VAR(60);
		}
		else if (av(key_5))
		{
			av(fps_max) = 0;
		}

		// Create movement vector
		vecForce.x = _VAR(force_x);
		vecForce.y = _VAR(force_y);
		vec_accelerate(&vecMove, &vecSpeed, &vecForce, _VAR(0.5));

		// Move kinematic
		physX3->MoveKinematic(kinematicEnt, &vecMove);

		// Draw FPS
		fps = 0.9f * fps + 0.1f / _FLOAT(av(time_frame)) * 16;
		draw_text(_chr(str_printf(NULL, "FPS : %d", (int)fps)), _VAR(0), _VAR(100), (COLOR *)vector(0, 0, _VAR(255)));

		// Draw Actor speed
		physx::PxVec3 actorSpeed = kine->getLinearVelocity();
		VECTOR kineSpeed;
		physX3->PxVec3ToVec(actorSpeed, &kineSpeed);
		draw_text(_chr(str_printf(NULL, "Speed : (%f, %f, %f)", kineSpeed.z, kineSpeed.y, kineSpeed.z)), _VAR(0), _VAR(120), (COLOR *)vector(0, 0, _VAR(255)));
	}

	physX3->Release();

	// Quit game engine
	engine_close();
}