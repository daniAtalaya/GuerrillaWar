#include "ModulePickUp.h"
#include "Application.h"

#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"

#include "pickUp.h"
#include "LanzLlamas.h"
#include "Fusil.h"
#include "Recluso.h"

#define SPAWN_MARGIN 50


ModulePickUp::ModulePickUp(bool startEnabled) : Module(startEnabled)
{
	for (uint i = 0; i < MAX_PICKS; ++i)
		pickUps[i] = nullptr;
}

ModulePickUp::~ModulePickUp()
{

}

bool ModulePickUp::Start()
{
	texture = App->textures->Load("Assets/Sprites/powerUps.png");
	//enemyDestroyedFx = App->audio->LoadFx("Assets/Fx/explosion.wav");

	return true;
}


Update_Status ModulePickUp::PreUpdate()
{
	// Remove all enemies scheduled for deletion
	for (uint i = 0; i < MAX_PICKS; ++i)
	{
		if (pickUps[i] != nullptr && pickUps[i]->pendingToDelete)
		{
			delete pickUps[i];
			pickUps[i] = nullptr;
		}
	}

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModulePickUp::Update()
{
	HandlePickUpSpawn();

	for (uint i = 0; i < MAX_PICKS; ++i)
	{
		if (pickUps[i] != nullptr)
			pickUps[i]->Update();
	}

	HandlePickUpDespawn();

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModulePickUp::PostUpdate()
{
	for (uint i = 0; i < MAX_PICKS; ++i)
	{
		if (pickUps[i] != nullptr)
			pickUps[i]->Draw();
	}

	return Update_Status::UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePickUp::CleanUp()
{
	LOG("Freeing all picks");

	for (uint i = 0; i < MAX_PICKS; ++i)
	{
		if (pickUps[i] != nullptr)
		{
			delete pickUps[i];
			pickUps[i] = nullptr;
		}
	}

	return true;
}

bool ModulePickUp::AddPick(Pick_Type type, int x, int y)
{
	bool ret = false;

	for (uint i = 0; i < MAX_PICKS; ++i)
	{
		if (spawnQueue[i].type == Pick_Type::NO_TYPE)
		{
			spawnQueue[i].type = type;
			spawnQueue[i].x = x;
			spawnQueue[i].y = y;
			ret = true;
			break;
		}
	}

	return ret;
}

void ModulePickUp::HandlePickUpSpawn()
{
	// Iterate all the enemies queue
	for (uint i = 0; i < MAX_PICKS; ++i)
	{
		if (spawnQueue[i].type != Pick_Type::NO_TYPE)
		{
			// Spawn a new enemy if the screen has reached a spawn position
			if (spawnQueue[i].x * SCREEN_SIZE < App->player->cameraGameplay.x + (App->player->cameraGameplay.w * SCREEN_SIZE) + SPAWN_MARGIN)
			{
				LOG("Spawning enemy at %d", spawnQueue[i].x * SCREEN_SIZE);

				SpawnpickUp(spawnQueue[i]);
				spawnQueue[i].type = Pick_Type::NO_TYPE; // Removing the newly spawned enemy from the queue
			}
		}
	}
}

void ModulePickUp::HandlePickUpDespawn()
{
	// Iterate existing enemies
	for (uint i = 0; i < MAX_PICKS; ++i)
	{
		if (pickUps[i] != nullptr)
		{
			// Delete the enemy when it has reached the end of the screen
			if (pickUps[i]->positionenemy.x * SCREEN_SIZE < (App->player->cameraGameplay.x) - SPAWN_MARGIN)
			{
				LOG("DeSpawning pick at %d", pickUps[i]->positionenemy.x * SCREEN_SIZE);

				pickUps[i]->SetToDelete();
			}
		}
	}
}

void ModulePickUp::SpawnpickUp(const PickSpawnpoint& info)
{
	// Find an empty slot in the enemies array
	for (uint i = 0; i < MAX_PICKS; ++i)
	{
		if (pickUps[i] == nullptr)
		{
			switch (info.type)
			{
			case Pick_Type::FUSIL:
				pickUps[i] = new Fusil(info.x, info.y);
				break;
			case Pick_Type::LANZALLAMAS:
				pickUps[i] = new LanzLlamas(info.x, info.y);
				break;
			case Pick_Type::RECLUSO:
				pickUps[i] = new recluso(info.x, info.y);
				break;
			}
			pickUps[i]->texture = texture;
			//pickUps[i]->destroyedFx = enemyDestroyedFx;
			break;
		}
	}
}

void ModulePickUp::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_PICKS; ++i)
	{
		if (pickUps[i] != nullptr && pickUps[i]->GetCollider() == c1)
		{
			pickUps[i]->OnCollision(c2); //Notify the enemy of a collision
			break;
		}
	}
}